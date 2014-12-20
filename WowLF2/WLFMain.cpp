//--------------------------------------------------------------------------------------
// File: WLFMain.cpp
//
// A 2D-ACT simulation on World of Warcraft.
// Sprites originally from the game Little Fighter 2 by Marti Wong & Starsky Wong.
//
// Feel free to use this code for reference.
//
// Author Name : Caleb C. Zhong (Mr.IroN)
// Author Blog : http://blog.calebzhong.net
//--------------------------------------------------------------------------------------

#include <windows.h>
#include <d3d11_1.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <dinput.h>
#include <d3d11sdklayers.h>

#include "resource.h"
#include "AEroEngine.h"

#include "Classes\WLFFileReader.h"
#include "Classes\WLFBuff.h"
#include "Classes\WLFSprite.h"
#include "Classes\WLFScene.h"

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
extern ID3D11InputLayout*					g_pDebugVertexLayout;
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
extern XMMATRIX								gm_TransformForHUD;

extern INT									gi_WindowWidth;
extern INT									gi_WindowHeight;

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
extern SpriteFont*							xtk_SpriteFont_Arial_10;

//--------------------------------------------------------------------------------------
// Project Global Variables
//--------------------------------------------------------------------------------------

SpriteFont*									xtk_SpriteFont_Arial_7;

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
HRESULT InitDirectInput();
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

	if( FAILED( AENSCore::InitDevice() ) )
	{
		AENSCore::CleanupDevice();
		return 0;
	}

	if ( FAILED( InitDirectInput() ) )
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


HRESULT InitDirectInput() {

	// Initialize DirectInput device
	HRESULT hr = DirectInput8Create(g_hInst, 0x0800, IID_IDirectInput8, (LPVOID*)&g_pDirectInput, NULL);
	if (FAILED(hr))
		return E_FAIL;

	// Initialize the keyboard
	hr = g_pDirectInput->CreateDevice(GUID_SysKeyboard, &g_pKeyboardDevice, NULL);
	if (FAILED(hr))
		return E_FAIL;
	g_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);

	// Set the cooperate level of our keyboard
	g_pKeyboardDevice->SetCooperativeLevel(g_hWnd, DISCL_BACKGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY);

	// Let the keyboard get authority
	g_pKeyboardDevice->Acquire();

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
// Load game resources (mainly .dds texture) and fonts
//--------------------------------------------------------------------------------------
void LoadGameResources() {

	WLFDataFileReader::readResources("Resources\\resources.txt");
	xtk_SpriteFont_Arial_7 = new SpriteFont(g_pd3dDevice, L"Resources\\arial_7.spritefont");

}

//--------------------------------------------------------------------------------------
// Initialize the game
//--------------------------------------------------------------------------------------
void InitGameplay() {

	AENSCore::AEInitialize();

	// Create Scene
	WLFShrineCaveScene* scene = new WLFShrineCaveScene(100);
	ae_SceneManager.addSceneAt(0, scene);
	scene->initialize();

	// Run a scene
	ae_SceneManager.runScene(0);

}


//--------------------------------------------------------------------------------------
// Update a frame
//--------------------------------------------------------------------------------------
void Update() {

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
	if( g_driverType == D3D_DRIVER_TYPE_REFERENCE ) {
		t += ( float )XM_PI * 0.0125f;
	}
	else {
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
void Render() {

	// Clear the back buffer
	g_pImmediateContext->ClearRenderTargetView( g_pRenderTargetView, Colors::Black );

	// Clear the depth buffer to 1.0 (max depth)
	g_pImmediateContext->ClearDepthStencilView( g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	// Change the view matrix by camera position
	XMVECTOR Eye = XMVectorSet((INT)(ae_Camera.getAdjustedFocus().x - (gi_WindowWidth / 2.0f)), (INT)(ae_Camera.getAdjustedFocus().y - (gi_WindowHeight / 2.0f)), 0.0f, 0.0f);
	XMVECTOR At = XMVectorSet((INT)(ae_Camera.getAdjustedFocus().x - (gi_WindowWidth / 2.0f)), (INT)(ae_Camera.getAdjustedFocus().y - (gi_WindowHeight / 2.0f)), 100.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	gm_View = XMMatrixLookAtLH(Eye, At, Up);

	// Calculate the overall transform matrix
	gm_Transform = gm_World * gm_View * gm_Projection;

	// Get and check the current active scene
	AEScene* activeScene = ae_SceneManager.getActiveScene();
	if (activeScene == nullptr) {
		AENSGameControl::exitGame("On gameplay: No active scene.");
	}

	// Render transformed things (non-HUD things)
	xtk_SpriteBatch->Begin(
		SpriteSortMode_BackToFront,
		g_pBlendState,
		g_pSamplerLinear,
		nullptr,
		g_pRasterizerState,
		nullptr, // TODO: Apply shaders
		gm_Transform
	);
	activeScene->render(AEScene::RENDER_BACKGROUND | AEScene::RENDER_SPRITES);
	xtk_SpriteBatch->End();

	// Render HUD (keeps stationary position on game window by reset transform)
	// Render debug information along with HUD
	xtk_SpriteBatch->Begin(
		SpriteSortMode_BackToFront,
		g_pBlendState,
		g_pSamplerLinear,
		nullptr,
		g_pRasterizerState,
		nullptr, // TODO: Apply shaders
		gm_TransformForHUD
	);
	activeScene->render(AEScene::RENDER_HUD);
	LPTSTR strSpriteCount = new TCHAR[1024];
	wsprintf(strSpriteCount, L"Sprite Count: %d", activeScene->getSpriteTable()->getHashCount());
	xtk_SpriteFont_Arial_10->DrawString(xtk_SpriteBatch, strSpriteCount, XMFLOAT2(-320.0f, 220.0f), XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
	xtk_SpriteBatch->End();

	// Present our back buffer to our front buffer
	g_pSwapChain->Present( 0, 0 );

}
