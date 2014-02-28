#include <d3d11_1.h>
#include <directxmath.h>
#include <dinput.h>
#include <vector>
#include "AEroEngine.h"

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE									g_hInst = nullptr;
HWND										g_hWnd = nullptr;
D3D_DRIVER_TYPE								g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL							g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*								g_pd3dDevice = nullptr;
ID3D11Device1*								g_pd3dDevice1 = nullptr;
ID3D11DeviceContext*						g_pImmediateContext = nullptr;
ID3D11DeviceContext1*						g_pImmediateContext1 = nullptr;
IDXGISwapChain*								g_pSwapChain = nullptr;
ID3D11RenderTargetView*						g_pRenderTargetView = nullptr;
ID3D11Texture2D*							g_pDepthStencil = nullptr;
ID3D11DepthStencilView*						g_pDepthStencilView = nullptr;
ID3D11VertexShader*							g_pVertexShader = nullptr;
ID3D11PixelShader*							g_pPixelShader = nullptr;
ID3D11InputLayout*							g_pVertexLayout = nullptr;
ID3D11Buffer*								g_pVertexBuffer = nullptr;
ID3D11Buffer*								g_pIndexBuffer = nullptr;
ID3D11Buffer*								g_pCBNeverChanges = nullptr;
ID3D11Buffer*								g_pCBChangeOnResize = nullptr;
ID3D11Buffer*								g_pCBChangesEveryFrame = nullptr;
ID3D11ShaderResourceView*					g_pTextureRV = nullptr;
ID3D11SamplerState*							g_pSamplerLinear = nullptr;
LPDIRECTINPUT8								g_pDirectInput = nullptr;
LPDIRECTINPUTDEVICE8						g_pKeyboardDevice = nullptr;
CHAR										g_pKeyStateBuffer[256] = { 0 };
XMMATRIX									g_World;
XMMATRIX									g_View;
XMMATRIX									g_Projection;
XMFLOAT4									g_vMeshColor( 1.0f, 1.0f, 1.0f, 1.0f );
std::vector<SimpleVertex>					vertices;
std::vector<INT>							indices;

//--------------------------------------------------------------------------------------
// AE Global Variables
//--------------------------------------------------------------------------------------
AEConstantTable<AEResource>					resourceTable(100);
AEConstantTable<AEObject>					objectTable(100);
AEHashedTable<AEAI>							aiTable(100);
AEHashedTable<AEParticleEmitter>			particleTable(1000);
AEBackgroundLibrary							bgLib;
AESceneManager								sceneManager;
AECamera									camera;