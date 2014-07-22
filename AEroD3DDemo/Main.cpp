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

#include "resource.h"
#include "AEroEngine.h"

#include "Classes\Sprites.h"
#include "Classes\Scenes.h"

using namespace DirectX;


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

extern XMMATRIX								gm_World;
extern XMMATRIX								gm_View;
extern XMMATRIX								gm_Projection;
extern XMMATRIX								gm_Transform;

LPDIRECTINPUT8								g_pDirectInput;
LPDIRECTINPUTDEVICE8						g_pKeyboardDevice;

//--------------------------------------------------------------------------------------
// AE Global Variables
//--------------------------------------------------------------------------------------
extern AEConstantTable<AEResource>			ae_ResourceTable;
extern AEConstantTable<AEObject>			ae_ObjectTable;
extern AEHashedTable<AEAI>					ae_AITable;
extern AEHashedTable<AEParticleEmitter>		ae_ParticleTable;
extern AEBackgroundLibrary					ae_BGLibrary;
extern AESceneManager						ae_SceneManager;
extern AECamera								ae_Camera;

//--------------------------------------------------------------------------------------
// DirectXTK Global Variables
//--------------------------------------------------------------------------------------
extern SpriteBatch*							xtk_SpriteBatch;
extern SpriteFont*							xtk_SpriteFont;


//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
HRESULT InitDevice();
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
void LoadGameResources();
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

	LoadGameResources();
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
	for (INT i = 0; i < ae_ResourceTable.getMaxElemCount(); i++) {
		if (ae_ResourceTable.isOccupied(i)) {
			ae_ResourceTable.getItem(i)->releaseTexture();
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
	g_hWnd = CreateWindow( L"AEroWindowClass", L"AEro Engine for Direct3D", WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
	   nullptr );
	if( !g_hWnd )
		return E_FAIL;

	ShowWindow( g_hWnd, nCmdShow );

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Read DirectInput device state
// Code by zhmxy555
// http://blog.csdn.net/zhmxy555/article/details/8547531
//--------------------------------------------------------------------------------------
VOID ReadInputDevice(IDirectInputDevice8* pDIDevice, LPVOID pBuffer, LONG lSize) {
	while (true) {
		if (FAILED(pDIDevice->Poll())) {
			AENSGameControl::exitGame("Failed when processing input.");
		}
		if (FAILED(pDIDevice->Acquire())) {
			AENSGameControl::exitGame("Failed when processing input.");
		}
		if SUCCEEDED(pDIDevice->GetDeviceState(lSize, pBuffer)) {
			break;
		}
	}
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
	descRasterizer.CullMode = D3D11_CULL_NONE;
	descRasterizer.FrontCounterClockwise = FALSE;
	descRasterizer.DepthBias = 0;
	descRasterizer.DepthBiasClamp = 0.0f;
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
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
	gm_World = XMMatrixIdentity();

	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet(-(width / 2.0f), -(height / 2.0f), 0.0f, 0.0f);
	XMVECTOR At = XMVectorSet(-(width / 2.0f), -(height / 2.0f), 100.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	gm_View = XMMatrixLookAtLH(Eye, At, Up);
	//gm_View = XMMatrixIdentity();

	// Initialize the projection matrix
	gm_Projection = XMMatrixIdentity();

	// Initialize DirectInput device
	hr = DirectInput8Create(g_hInst, 0x0800, IID_IDirectInput8, (LPVOID*)&g_pDirectInput, NULL);
	if( FAILED( hr ) )
		return E_FAIL;

	// Initialize the keyboard
	hr = g_pDirectInput->CreateDevice(GUID_SysKeyboard, &g_pKeyboardDevice, NULL);
	if( FAILED( hr ) )  
		return E_FAIL;
	g_pKeyboardDevice->SetDataFormat( &c_dfDIKeyboard );

	// Set the cooperate level of our keyboard
	g_pKeyboardDevice->SetCooperativeLevel( g_hWnd, DISCL_BACKGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY );

	// Let the keyboard get authority
	g_pKeyboardDevice->Acquire();

	xtk_SpriteBatch = new SpriteBatch(g_pImmediateContext);
	xtk_SpriteFont = new SpriteFont(g_pd3dDevice, L"Resources\\arial.spritefont");

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
// Load game resources (mainly .dds texture)
//--------------------------------------------------------------------------------------
void LoadGameResources() {

	// Gedama Sprite
	AERO_RESOURCE_DESC descRes;
	descRes.rid = 0;
	descRes.rtype = RES_5x10;
	descRes.cellW = 80;
	descRes.cellH = 70;
	HRESULT hr = CreateDDSTextureFromFile(g_pd3dDevice, L"Resources\\gedama_0.dds", nullptr, &(descRes.tex));
	if (FAILED(hr)) {
		AENSGameControl::exitGame("On loading texture: Texture load failed.");
	}
	ae_ResourceTable.addAt(descRes.rid, new AEResource(descRes));

	// Starsky Background
	descRes.rid = 1;
	descRes.rtype = RES_1x1;
	descRes.cellW = 1000;
	descRes.cellH = 771;
	hr = CreateDDSTextureFromFile(g_pd3dDevice, L"Resources\\starsky.dds", nullptr, &(descRes.tex));
	if (FAILED(hr)) {
		AENSGameControl::exitGame("On loading texture: Texture load failed.");
	}
	ae_ResourceTable.addAt(descRes.rid, new AEResource(descRes));

	// Japanese Fighter Sprite
	descRes.rid = 2;
	descRes.rtype = RES_1x5;
	descRes.cellW = 50;
	descRes.cellH = 50;
	hr = CreateDDSTextureFromFile(g_pd3dDevice, L"Resources\\jfighter_0.dds", nullptr, &(descRes.tex));
	if (FAILED(hr)) {
		AENSGameControl::exitGame("On loading texture: Texture load failed.");
	}
	ae_ResourceTable.addAt(descRes.rid, new AEResource(descRes));

	// Bullets Sprite
	descRes.rid = 3;
	descRes.rtype = RES_1x5;
	descRes.cellW = 17;
	descRes.cellH = 17;
	hr = CreateDDSTextureFromFile(g_pd3dDevice, L"Resources\\bullet_0.dds", nullptr, &(descRes.tex));
	if (FAILED(hr)) {
		AENSGameControl::exitGame("On loading texture: Texture load failed.");
	}
	ae_ResourceTable.addAt(descRes.rid, new AEResource(descRes));

	// Rocket Smoke Effect
	descRes.rid = 4;
	descRes.rtype = RES_1x10;
	descRes.cellW = 20;
	descRes.cellH = 20;
	hr = CreateDDSTextureFromFile(g_pd3dDevice, L"Resources\\bullet_smoke.dds", nullptr, &(descRes.tex));
	if (FAILED(hr)) {
		AENSGameControl::exitGame("On loading texture: Texture load failed.");
	}
	ae_ResourceTable.addAt(descRes.rid, new AEResource(descRes));

	// Enemy Turret
	descRes.rid = 5;
	descRes.rtype = RES_1x5;
	descRes.cellW = 63;
	descRes.cellH = 63;
	hr = CreateDDSTextureFromFile(g_pd3dDevice, L"Resources\\turret_0.dds", nullptr, &(descRes.tex));
	if (FAILED(hr)) {
		AENSGameControl::exitGame("On loading texture: Texture load failed.");
	}
	ae_ResourceTable.addAt(descRes.rid, new AEResource(descRes));

	// Helicopter Sprite
	descRes.rid = 6;
	descRes.rtype = RES_1x5;
	descRes.cellW = 50;
	descRes.cellH = 20;
	hr = CreateDDSTextureFromFile(g_pd3dDevice, L"Resources\\heli_0.dds", nullptr, &(descRes.tex));
	if (FAILED(hr)) {
		AENSGameControl::exitGame("On loading texture: Texture load failed.");
	}
	ae_ResourceTable.addAt(descRes.rid, new AEResource(descRes));

	// Platform background
	descRes.rid = 7;
	descRes.rtype = RES_1x1;
	descRes.cellW = 640;
	descRes.cellH = 480;
	hr = CreateDDSTextureFromFile(g_pd3dDevice, L"Resources\\platform_bg.dds", nullptr, &(descRes.tex));
	if (FAILED(hr)) {
		AENSGameControl::exitGame("On loading texture: Texture load failed.");
	}
	ae_ResourceTable.addAt(descRes.rid, new AEResource(descRes));

	// Flak Cannon Sprite
	descRes.rid = 8;
	descRes.rtype = RES_1x5;
	descRes.cellW = 50;
	descRes.cellH = 35;
	hr = CreateDDSTextureFromFile(g_pd3dDevice, L"Resources\\flak_0.dds", nullptr, &(descRes.tex));
	if (FAILED(hr)) {
		AENSGameControl::exitGame("On loading texture: Texture load failed.");
	}
	ae_ResourceTable.addAt(descRes.rid, new AEResource(descRes));

	// Flak Cannon Muzzle Flash Sprite
	descRes.rid = 9;
	descRes.rtype = RES_1x5;
	descRes.cellW = 50;
	descRes.cellH = 25;
	hr = CreateDDSTextureFromFile(g_pd3dDevice, L"Resources\\flak_muzzleflash.dds", nullptr, &(descRes.tex));
	if (FAILED(hr)) {
		AENSGameControl::exitGame("On loading texture: Texture load failed.");
	}
	ae_ResourceTable.addAt(descRes.rid, new AEResource(descRes));

	// Big Smoke Sprite
	descRes.rid = 10;
	descRes.rtype = RES_2x5;
	descRes.cellW = 60;
	descRes.cellH = 60;
	hr = CreateDDSTextureFromFile(g_pd3dDevice, L"Resources\\big_smoke.dds", nullptr, &(descRes.tex));
	if (FAILED(hr)) {
		AENSGameControl::exitGame("On loading texture: Texture load failed.");
	}
	ae_ResourceTable.addAt(descRes.rid, new AEResource(descRes));

	// Explosions Sprite
	descRes.rid = 11;
	descRes.rtype = RES_5x10;
	descRes.cellW = 50;
	descRes.cellH = 50;
	hr = CreateDDSTextureFromFile(g_pd3dDevice, L"Resources\\explosions.dds", nullptr, &(descRes.tex));
	if (FAILED(hr)) {
		AENSGameControl::exitGame("On loading texture: Texture load failed.");
	}
	ae_ResourceTable.addAt(descRes.rid, new AEResource(descRes));

}

//--------------------------------------------------------------------------------------
// Initialize the game
//--------------------------------------------------------------------------------------
void InitGameplay() {

	AENSCore::AEInitialize();

	// Scene 0 - General Sample Scene
	GeneralSampleScene* generalSampleScene = new GeneralSampleScene(nullptr, nullptr, new AEHashedTable<AESprite>(100), nullptr);
	ae_SceneManager.addSceneAt(0, generalSampleScene);

	// Scene 1 - Vertical Scroller Scene
	VerticalScrollerScene* verticalScrollerScene = new VerticalScrollerScene(nullptr, nullptr, new AEHashedTable<AESprite>(1000), nullptr);
	ae_SceneManager.addSceneAt(1, verticalScrollerScene);

	// Scene 2 - Side Scroller Scene with Platform
	SideScrollerPlatformScene* sideScrollerPlatformScene = new SideScrollerPlatformScene(nullptr, nullptr, new AEHashedTable<AESprite>(1000), nullptr);
	ae_SceneManager.addSceneAt(2, sideScrollerPlatformScene);

	// Initialize scenes
	generalSampleScene->initialize();
	verticalScrollerScene->initialize();
	sideScrollerPlatformScene->initialize();

	// Run a scene
	ae_SceneManager.runScene(0);

}


//--------------------------------------------------------------------------------------
// Update a frame
//--------------------------------------------------------------------------------------
void Update()
{

	// Get input and update the scene
	AEScene* activeScene = ae_SceneManager.getActiveScene();
	if (activeScene == NULL) {
		AENSGameControl::exitGame("On gameplay: No active scene.");
	}
	ReadInputDevice(g_pKeyboardDevice, (LPVOID)(activeScene->keyStateBuffer), sizeof(activeScene->keyStateBuffer));
	activeScene->processInput();
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

	// Clear the back buffer
	g_pImmediateContext->ClearRenderTargetView( g_pRenderTargetView, Colors::Black );

	// Clear the depth buffer to 1.0 (max depth)
	g_pImmediateContext->ClearDepthStencilView( g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	// Calculate the overall transform matrix
	gm_Transform = gm_World * gm_View * gm_Projection;

	// Begin rendering
	xtk_SpriteBatch->Begin(
		SpriteSortMode_BackToFront,
		g_pBlendState,
		g_pSamplerLinear,
		nullptr,
		g_pRasterizerState,
		nullptr, // TODO: Apply shaders
		gm_Transform
	);

	AEScene* activeScene = ae_SceneManager.getActiveScene();
	if ( activeScene == nullptr ) {
		AENSGameControl::exitGame("On gameplay: No active scene.");
	}
	activeScene->render();

	LPTSTR strSpriteCount = new TCHAR[1024];
	wsprintf(strSpriteCount, L"Sprite Count: %d", activeScene->getSpriteTable()->getHashCount());
	xtk_SpriteFont->DrawString(xtk_SpriteBatch, strSpriteCount, XMFLOAT2(-320.0f, -240.0f));
	xtk_SpriteBatch->End();

	// Present our back buffer to our front buffer
	g_pSwapChain->Present( 0, 0 );

}
