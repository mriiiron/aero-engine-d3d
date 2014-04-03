//--------------------------------------------------------------------------------------
// File: AEMain.cpp
//
// This application demonstrates texturing
//
// http://msdn.microsoft.com/en-us/library/windows/apps/ff729724.aspx
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#pragma comment(lib, "dinput8.lib")

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <dinput.h>
#include <d3d11sdklayers.h>
#include <vector>
#include "DDSTextureLoader.h"
#include "resource.h"
#include "AEroEngine.h"

#include "Classes\Scenes.h"

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------

struct CBNeverChanges
{
	XMMATRIX mView;
};

struct CBChangeOnResize
{
	XMMATRIX mProjection;
};

struct CBChangesEveryFrame
{
	XMMATRIX mWorld;
};


//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
extern HINSTANCE							g_hInst;
extern HWND									g_hWnd;
extern D3D_DRIVER_TYPE						g_driverType;
extern D3D_FEATURE_LEVEL					g_featureLevel;
extern ID3D11Device*						g_pd3dDevice;
extern ID3D11Device1*						g_pd3dDevice1;
extern ID3D11DeviceContext*					g_pImmediateContext;
extern ID3D11DeviceContext1*				g_pImmediateContext1;
extern IDXGISwapChain*						g_pSwapChain;
extern ID3D11RenderTargetView*				g_pRenderTargetView;
extern ID3D11Texture2D*						g_pDepthStencil;
extern ID3D11DepthStencilView*				g_pDepthStencilView;
extern ID3D11BlendState*					g_pBlendState;
extern ID3D11RasterizerState*				g_pRasterizerState;
extern ID3D11VertexShader*					g_pVertexShader;
extern ID3D11PixelShader*					g_pPixelShader;
extern ID3D11InputLayout*					g_pVertexLayout;
extern ID3D11Buffer*						g_pVertexBuffer;
extern ID3D11Buffer*						g_pIndexBuffer;
extern ID3D11Buffer*						g_pCBNeverChanges;
extern ID3D11Buffer*						g_pCBChangeOnResize;
extern ID3D11Buffer*						g_pCBChangesEveryFrame;
extern ID3D11SamplerState*					g_pSamplerLinear;
extern ID3D11Debug*							g_pDebug;

LPDIRECTINPUT8								g_pDirectInput;
LPDIRECTINPUTDEVICE8						g_pKeyboardDevice;

extern CHAR									g_pKeyStateBuffer[256];
extern XMMATRIX								g_World;
extern XMMATRIX								g_View;
extern XMMATRIX								g_Projection;
extern std::vector<SimpleVertex>			vertices;
extern std::vector<INT>						indices;

//--------------------------------------------------------------------------------------
// AE Global Variables
//--------------------------------------------------------------------------------------
extern AEConstantTable<AEResource>			resourceTable;
extern AEConstantTable<AEObject>			objectTable;
extern AEHashedTable<AEAI>					aiTable;
extern AEHashedTable<AEParticleEmitter>		particleTable;
extern AEBackgroundLibrary					bgLib;
extern AESceneManager						sceneManager;
extern AECamera								camera;


//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
HRESULT InitDevice();
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
void InitGameplay();
void Update();
void Render();


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
		return 0;

	if( FAILED( InitDevice() ) )
	{
		AENSCore::CleanupDevice();
		return 0;
	}

	InitGameplay();

	// Main message loop
	MSG msg = {0};
	while( WM_QUIT != msg.message )
	{
		if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			Update();
			Render();
		}
	}

	// Release objects, including textures stored in AEResources
	for (INT i = 0; i < resourceTable.getMaxElemCount(); i++) {
		if (resourceTable.isOccupied(i)) {
			resourceTable.getItem(i)->releaseTexture();
		}
	}
	AENSCore::CleanupDevice();

	return ( int )msg.wParam;
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
	wcex.hCursor = LoadCursor( nullptr, IDC_ARROW );
	wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"AEroWindowClass";
	wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
	if( !RegisterClassEx( &wcex ) )
		return E_FAIL;

	// Create window
	g_hInst = hInstance;
	RECT rc = { 0, 0, 640, 480 };
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
	g_hWnd = CreateWindow( L"AEroWindowClass", L"AEro Engine for Direct 3D", WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
	   nullptr );
	if( !g_hWnd )
		return E_FAIL;

	ShowWindow( g_hWnd, nCmdShow );

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DCompile
//
// With VS 11, we could load up prebuilt .cso files instead...
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile( szFileName, nullptr, nullptr, szEntryPoint, szShaderModel, 
	dwShaderFlags, 0, ppBlobOut, &pErrorBlob );
	if( FAILED(hr) )
	{
		if( pErrorBlob )
		{
			OutputDebugStringA( reinterpret_cast<const char*>( pErrorBlob->GetBufferPointer() ) );
			pErrorBlob->Release();
		}
		return hr;
	}
	if( pErrorBlob ) pErrorBlob->Release();

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect( g_hWnd, &rc );
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
	#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE( driverTypes );

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

	// Create device and swap Chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof( sd ) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain( nullptr, g_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
		D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );

		if ( hr == E_INVALIDARG )
		{
			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
			hr = D3D11CreateDeviceAndSwapChain( nullptr, g_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
			D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );
		}

		if( SUCCEEDED( hr ) )
			break;
	}
	if( FAILED( hr ) )
		return hr;

	// Enable debug layer
	hr = g_pd3dDevice->QueryInterface( IID_PPV_ARGS( &g_pDebug ) );
	if( FAILED( hr ) )
		return hr;

	// Obtain the Direct3D 11.1 versions if available
	hr = g_pd3dDevice->QueryInterface( __uuidof( ID3D11Device1 ), reinterpret_cast<void**>( &g_pd3dDevice1 ) );
	if ( SUCCEEDED(hr) )
	{
		(void)g_pImmediateContext->QueryInterface( __uuidof( ID3D11DeviceContext1 ), reinterpret_cast<void**>( &g_pImmediateContext1 ) );
	}

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
	if( FAILED( hr ) )
		return hr;

	hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, nullptr, &g_pRenderTargetView );
	pBackBuffer->Release();
	if( FAILED( hr ) )
		return hr;

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory( &descDepth, sizeof(descDepth) );
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = g_pd3dDevice->CreateTexture2D( &descDepth, nullptr, &g_pDepthStencil );
	if( FAILED( hr ) )
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory( &descDSV, sizeof(descDSV) );
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView( g_pDepthStencil, &descDSV, &g_pDepthStencilView );
	if( FAILED( hr ) )
		return hr;

	g_pImmediateContext->OMSetRenderTargets( 1, &g_pRenderTargetView, g_pDepthStencilView );

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports( 1, &vp );

	// Setup blend options
	D3D11_BLEND_DESC descBlend;  
	descBlend.AlphaToCoverageEnable = FALSE;
	descBlend.IndependentBlendEnable = FALSE;
	descBlend.RenderTarget[0].BlendEnable = TRUE;  
	descBlend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;  
	descBlend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;  
	descBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;  
	descBlend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;  
	descBlend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;  
	descBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;  
	descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;  
	hr = g_pd3dDevice->CreateBlendState( &descBlend, &g_pBlendState );
	if( FAILED( hr ) )
	{
		MessageBox( nullptr,
		L"Cannot create blend state.", L"Error", MB_OK );
		return hr;
	}
	g_pImmediateContext->OMSetBlendState(g_pBlendState, nullptr, 0xFFFFFFFF);

	// Setup rasterizer options
	D3D11_RASTERIZER_DESC descRasterizer;
	descRasterizer.FillMode = D3D11_FILL_SOLID;
	descRasterizer.CullMode = D3D11_CULL_BACK;
	descRasterizer.FrontCounterClockwise = FALSE;
	descRasterizer.DepthBias = 1;
	descRasterizer.DepthBiasClamp = 1.0f;
	descRasterizer.SlopeScaledDepthBias = 0.0f;
	descRasterizer.DepthClipEnable = TRUE;
	descRasterizer.ScissorEnable = FALSE;
	descRasterizer.MultisampleEnable = FALSE;
	descRasterizer.AntialiasedLineEnable = FALSE;
	hr = g_pd3dDevice->CreateRasterizerState( &descRasterizer, &g_pRasterizerState );
	if( FAILED( hr ) )
	{
		MessageBox( nullptr,
		L"Cannot create rasterizer state.", L"Error", MB_OK );
		return hr;
	}
	g_pImmediateContext->RSSetState( g_pRasterizerState );

	// Compile the vertex shader
	ID3DBlob* pVSBlob = nullptr;
	hr = CompileShaderFromFile( L"AEroD3D.fx", "VS", "vs_4_0", &pVSBlob );
	if( FAILED( hr ) )
	{
		MessageBox( nullptr,
		L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
		return hr;
	}

	// Create the vertex shader
	hr = g_pd3dDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader );
	if( FAILED( hr ) )
	{
		pVSBlob->Release();
		return hr;
	}

	// Define the vertex layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE( layout );

	// Create the vertex layout
	hr = g_pd3dDevice->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pVertexLayout );
	pVSBlob->Release();
	if( FAILED( hr ) )
		return hr;

	// Set the vertex layout
	g_pImmediateContext->IASetInputLayout( g_pVertexLayout );

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	hr = CompileShaderFromFile( L"AEroD3D.fx", "PS", "ps_4_0", &pPSBlob );
	if( FAILED( hr ) )
	{
		MessageBox( nullptr,
		L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
		return hr;
	}

	// Create the pixel shader
	hr = g_pd3dDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader );
	pPSBlob->Release();
	if( FAILED( hr ) )
		return hr;

	// Create vertex buffer as dynamic (CPU write only / GPU read only)
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof( SimpleVertex ) * 1000;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	hr = g_pd3dDevice->CreateBuffer( &bd, nullptr, &g_pVertexBuffer );
	if( FAILED( hr ) ) {
		return hr;
	}

	// Set vertex buffer
	UINT stride = sizeof( SimpleVertex );
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers( 0, 1, &g_pVertexBuffer, &stride, &offset );

	// Create index buffer as dynamic (CPU write only / GPU read only)
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof( WORD ) * 1500;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	hr = g_pd3dDevice->CreateBuffer( &bd, nullptr, &g_pIndexBuffer );
	if( FAILED( hr ) ) {
		return hr;
	}

	// Set index buffer
	g_pImmediateContext->IASetIndexBuffer( g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );

	// Set primitive topology
	g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// Create the constant buffers
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBNeverChanges);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer( &bd, nullptr, &g_pCBNeverChanges );
	if( FAILED( hr ) )
		return hr;

	bd.ByteWidth = sizeof(CBChangeOnResize);
	hr = g_pd3dDevice->CreateBuffer( &bd, nullptr, &g_pCBChangeOnResize );
	if( FAILED( hr ) )
		return hr;

	bd.ByteWidth = sizeof(CBChangesEveryFrame);
	hr = g_pd3dDevice->CreateBuffer( &bd, nullptr, &g_pCBChangesEveryFrame );
	if( FAILED( hr ) )
		return hr;

	// Create the sampler state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory( &sampDesc, sizeof(sampDesc) );
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = g_pd3dDevice->CreateSamplerState( &sampDesc, &g_pSamplerLinear );
	if( FAILED( hr ) )
		return hr;

	// Initialize the world matrix
	g_World = XMMatrixIdentity();

	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet( 0.0f, 0.0f, - (FLOAT)height / 2.0f, 0.0f );
	XMVECTOR At = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
	XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	g_View = XMMatrixLookAtLH( Eye, At, Up );

	CBNeverChanges cbNeverChanges;
	cbNeverChanges.mView = XMMatrixTranspose( g_View );
	g_pImmediateContext->UpdateSubresource( g_pCBNeverChanges, 0, nullptr, &cbNeverChanges, 0, 0 );

	// Initialize the projection matrix
	g_Projection = XMMatrixPerspectiveFovLH( XM_PIDIV2, width / (FLOAT)height, 0.01f, 1000.0f );

	CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.mProjection = XMMatrixTranspose( g_Projection );
	g_pImmediateContext->UpdateSubresource( g_pCBChangeOnResize, 0, nullptr, &cbChangesOnResize, 0, 0 );

	// Initialize DirectInput device
	if( FAILED( DirectInput8Create( g_hInst, 0x0800, IID_IDirectInput8, (LPVOID*)&g_pDirectInput, NULL ) ) )
		return E_FAIL;

	// Initialize the keyboard
	if( FAILED( g_pDirectInput->CreateDevice( GUID_SysKeyboard, &g_pKeyboardDevice, NULL ) ) )  
		return E_FAIL;
	g_pKeyboardDevice->SetDataFormat( &c_dfDIKeyboard );

	// Set the cooperate level of our keyboard
	g_pKeyboardDevice->SetCooperativeLevel( g_hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY );

	// Let the keyboard
	g_pKeyboardDevice->Acquire();

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch( message )
	{
		case WM_PAINT:
		hdc = BeginPaint( hWnd, &ps );
		EndPaint( hWnd, &ps );
		break;

		case WM_DESTROY:
		PostQuitMessage( 0 );
		break;

		default:
		return DefWindowProc( hWnd, message, wParam, lParam );
	}

	return 0;
}



//--------------------------------------------------------------------------------------
// Read DirectInput device state
// Code by zhmxy555
// http://blog.csdn.net/zhmxy555/article/details/8547531
//--------------------------------------------------------------------------------------
BOOLEAN Device_Read( IDirectInputDevice8* pDIDevice, LPVOID pBuffer, LONG lSize )  
{  
	HRESULT hr;  
	while( true )  
	{  
		pDIDevice->Poll();
		pDIDevice->Acquire();
		if( SUCCEEDED( hr = pDIDevice->GetDeviceState(lSize, pBuffer) ) )
			break;  
		if( hr != DIERR_INPUTLOST || hr != DIERR_NOTACQUIRED )
			return FALSE;  
		if( FAILED( pDIDevice->Acquire() ) )
			return FALSE;  
	}  
	return TRUE;  
}


//--------------------------------------------------------------------------------------
// Initialize the game
//--------------------------------------------------------------------------------------
void InitGameplay()
{

	// Load sprite resources
	AERO_RESOURCE_DESC descRes;
	descRes.rid = 0;
	descRes.rtype = RES_5x10;
	descRes.cellW = 80;
	descRes.cellH = 70;
	HRESULT hr = CreateDDSTextureFromFile(g_pd3dDevice, L"Resources\\gedama_0.dds", nullptr, &(descRes.tex) );
	if (FAILED(hr)) {
		AENSGameControl::exitGame("On loading texture: Texture load failed.");
	}
	AEResource* res_1 = new AEResource(descRes);
	resourceTable.addAt(descRes.rid, res_1);

	// Load background resources
	descRes.rid = 1;
	descRes.rtype = RES_1x1;
	descRes.cellW = 1000;
	descRes.cellH = 771;
	hr = CreateDDSTextureFromFile(g_pd3dDevice, L"Resources\\starsky.dds", nullptr, &(descRes.tex) );
	if (FAILED(hr)) {
		AENSGameControl::exitGame("On loading texture: Texture load failed.");
	}
	AEResource* res_2 = new AEResource(descRes);
	resourceTable.addAt(descRes.rid, res_2);

	// Create Frame 00
	AERO_FRAME_DESC descFrame;
	descFrame.res = resourceTable.getItem(0);
	descFrame.centerx = 40;
	descFrame.centery = 10;
	descFrame.dvx = 0;
	descFrame.dvy = 0;
	descFrame.imgOffset = 7;
	descFrame.imgCells = 1;
	descFrame.shiftx = 0;
	descFrame.shifty = 0;
	AEFrame* frame_00 = new AEFrame(descFrame);

	// Create Frame 01
	descFrame.imgOffset = 8;
	AEFrame* frame_01 = new AEFrame(descFrame);

	// Create Frame 02
	descFrame.imgOffset = 9;
	AEFrame* frame_02 = new AEFrame(descFrame);

	// Create Frame 03
	descFrame.imgOffset = 8;
	AEFrame* frame_03 = new AEFrame(descFrame);
	
	// Create animation 0
	AERO_ANIMATION_DESC descAnim;
	descAnim.frameCount = 4;
	descAnim.isAnimLoop = TRUE;
	descAnim.next = 0;
	descAnim.state = 0;
	descAnim.timeToLive = -1;
	AEAnimation* anim_0 = new AEAnimation(descAnim);

	// Assign frames to animation 0
	anim_0->addFrame(0, frame_00, 15);
	anim_0->addFrame(1, frame_01, 30);
	anim_0->addFrame(2, frame_02, 45);
	anim_0->addFrame(3, frame_03, 60);

	// Create Frame 10
	descFrame.imgOffset = 10;
	AEFrame* frame_10 = new AEFrame(descFrame);

	// Create Frame 11
	descFrame.imgOffset = 11;
	AEFrame* frame_11 = new AEFrame(descFrame);

	// Create Frame 12
	descFrame.imgOffset = 12;
	AEFrame* frame_12 = new AEFrame(descFrame);

	// Create animation 1
	descAnim.frameCount = 3;
	descAnim.isAnimLoop = FALSE;
	descAnim.next = 0;
	descAnim.state = 0;
	descAnim.timeToLive = -1;
	AEAnimation* anim_1 = new AEAnimation(descAnim);

	// Assign frames to animation 1
	anim_1->addFrame(0, frame_10, 5);
	anim_1->addFrame(1, frame_11, 10);
	anim_1->addFrame(2, frame_12, 100);

	// Create objects
	AERO_OBJECT_DESC descObj;
	descObj.oid = 0;
	descObj.name = "Gedama";
	descObj.otype = OBJ_CHARACTER;
	AEObject* obj = new AEObject(descObj);

	// Assign animations to objects
	obj->addAnim(0, anim_0);
	obj->addAnim(1, anim_1);

	// Add objects to object table
	objectTable.addAt(0, obj);

	// Create BG layer animation. Only one frame (a static image) in this case
	AEBGLayerFrame* bgLayerFrame = new AEBGLayerFrame(resourceTable.getItem(1), 0);
	AERO_BGLAYERANIM_DESC descBGLayerAnim;
	descBGLayerAnim.frameCount = 1;
	AEBGLayerAnim* bgLayerAnim = new AEBGLayerAnim(descBGLayerAnim);
	bgLayerAnim->addFrame(0, bgLayerFrame, 1000);

	// Create BG
	AERO_BACKGROUND_DESC descBG;
	descBG.name = "Starsky";
	AEBackground* bg = new AEBackground(descBG);

	// Add BG layer animation to BG
	bg->addAnimAt(0, bgLayerAnim);

	// Create BG layer
	AERO_BGLAYER_DESC descBGLayer;
	descBGLayer.depth = 0;
	descBGLayer.locX = -500.0f;
	descBGLayer.locY = -385.5f;
	descBGLayer.width = 1000;
	descBGLayer.height = 771;
	AEBGLayer* bgLayer = new AEBGLayer(descBGLayer);
	
	// Add BG layer anim reference to layer
	AEBGAnimRef* animRef = new AEBGAnimRef(0, 0, 0);
	bgLayer->addAnimRef(animRef);

	// Add BG layers to BG
	bg->addLayer(bgLayer);

	// Add BG to BG library
	bgLib.add(bg);

	// Create scene with sprite table, background and HUD
	AEHashedTable<AESprite>* spriteTable = new AEHashedTable<AESprite>(100);
	AEHeadUpDisplay* hud = new AEHeadUpDisplay();
	TemplateScene* scene = new TemplateScene(bgLib.get(0), spriteTable, hud);
	sceneManager.addSceneAt(0, scene);

	// Create a sprite
	AERO_SPRITE_DESC descSpr;
	descSpr.obj = objectTable.getItem(0);
	descSpr.team = 0;
	descSpr.action = 0;
	descSpr.facing = 0;
	descSpr.cx = 0.0f;
	descSpr.cy = 0.0f;
	AESprite* spr_1 = new AESprite(descSpr);
	spr_1->rotateDeg(45);

	// Create another sprite
	descSpr.facing = 0;
	descSpr.cx = 60.0f;
	descSpr.cy = 20.0f;
	AESprite* spr_2 = new AESprite(descSpr);

	// Create another sprite
	descSpr.facing = 0;
	descSpr.cx = 80.0f;
	descSpr.cy = 15.0f;
	AESprite* spr_3 = new AESprite(descSpr);

	// Run the scene
	sceneManager.runScene(0);

	// Add sprites to the scene
	sceneManager.getActiveScene()->addSprite(spr_1);
	sceneManager.getActiveScene()->addSprite(spr_2);
	sceneManager.getActiveScene()->addSprite(spr_3);

}


//--------------------------------------------------------------------------------------
// Update a frame
//--------------------------------------------------------------------------------------
void Update()
{
	// Get and process keyboard state
	ZeroMemory( &g_pKeyStateBuffer, sizeof(g_pKeyStateBuffer) );
	Device_Read( g_pKeyboardDevice, (LPVOID)g_pKeyStateBuffer, sizeof( g_pKeyStateBuffer ) );
	if (g_pKeyStateBuffer[DIK_A] & 0x80) {
		AESprite* pGedama = sceneManager.getActiveScene()->getSpriteTable()->getItemByHash(0);
		pGedama->changeAction(1);
	}

	// Update the scene
	AEScene* activeScene = sceneManager.getActiveScene();
	if ( activeScene == NULL ) {
		AENSGameControl::exitGame("On gameplay: No active scene.");
	}
	activeScene->update();

	// Update our time
	static float t = 0.0f;
	if( g_driverType == D3D_DRIVER_TYPE_REFERENCE )
	{
		t += ( float )XM_PI * 0.0125f;
	}
	else
	{
		static ULONGLONG timeStart = 0;
		static ULONGLONG timeLast = 0;
		ULONGLONG timeCur = GetTickCount64();
		if( timeStart == 0 ) {
			timeStart = timeLast = timeCur;
		}
		else {
			ULONGLONG timeElapsed = timeCur - timeLast;
			LONGLONG timeSleep = 1000 / 60 - timeElapsed;
			if ( timeSleep < 2 ) {
				timeSleep = 2;
			}
			Sleep((DWORD)timeSleep);
			t = ( timeCur - timeStart ) / 1000.0f;
			timeLast = timeCur;
		}
	}

}


//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void Render()
{
	//
	// Clear the back buffer
	//
	g_pImmediateContext->ClearRenderTargetView( g_pRenderTargetView, Colors::Black );

	//
	// Clear the depth buffer to 1.0 (max depth)
	//
	g_pImmediateContext->ClearDepthStencilView( g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	//
	// Update variables that change once per frame
	//
	CBChangesEveryFrame cb;
	cb.mWorld = XMMatrixTranspose( g_World );
	g_pImmediateContext->UpdateSubresource( g_pCBChangesEveryFrame, 0, nullptr, &cb, 0, 0 );

	//
	// Draw the scene to buffers (vertex/index)
	//
	AEScene* activeScene = sceneManager.getActiveScene();
	if ( activeScene == NULL ) {
		AENSGameControl::exitGame("On gameplay: No active scene.");
	}
	activeScene->paint();

	//
	// Render
	//
	g_pImmediateContext->VSSetShader( g_pVertexShader, nullptr, 0 );
	g_pImmediateContext->VSSetConstantBuffers( 0, 1, &g_pCBNeverChanges );
	g_pImmediateContext->VSSetConstantBuffers( 1, 1, &g_pCBChangeOnResize );
	g_pImmediateContext->VSSetConstantBuffers( 2, 1, &g_pCBChangesEveryFrame );
	g_pImmediateContext->PSSetShader( g_pPixelShader, nullptr, 0 );
	g_pImmediateContext->PSSetConstantBuffers( 2, 1, &g_pCBChangesEveryFrame );
	g_pImmediateContext->PSSetSamplers( 0, 1, &g_pSamplerLinear );
	// for ( int i = 0; i < resourceTable.getMaxElemCount(); i++ ) {
	for ( int i = resourceTable.getMaxElemCount() - 1; i >= 0; i-- ) {
		if ( resourceTable.isOccupied(i) ) {
			AEResource* res = resourceTable.getItem(i);
			if (!res->isBufferEmpty()) {
				res->render();
				res->clearRenderBuffer();
			}
		}
	}

	//
	// Present our back buffer to our front buffer
	//
	g_pSwapChain->Present( 0, 0 );
}
