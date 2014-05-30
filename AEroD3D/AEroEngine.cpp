#include <d3d11_1.h>
#include <directxmath.h>
#include <d3d11sdklayers.h>

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
ID3D11BlendState*							g_pBlendState = nullptr;
ID3D11RasterizerState*						g_pRasterizerState = nullptr;
ID3D11VertexShader*							g_pVertexShader = nullptr;
ID3D11PixelShader*							g_pPixelShader = nullptr;
ID3D11InputLayout*							g_pVertexLayout = nullptr;
ID3D11Buffer*								g_pVertexBuffer = nullptr;
ID3D11Buffer*								g_pIndexBuffer = nullptr;
ID3D11Buffer*								g_pCBNeverChanges = nullptr;
ID3D11Buffer*								g_pCBChangeOnResize = nullptr;
ID3D11Buffer*								g_pCBChangesEveryFrame = nullptr;
ID3D11SamplerState*							g_pSamplerLinear = nullptr;
ID3D11Debug*								g_pDebug = nullptr;
XMMATRIX									g_World;
XMMATRIX									g_View;
XMMATRIX									g_Projection;
XMFLOAT4									g_vMeshColor( 1.0f, 1.0f, 1.0f, 1.0f );

//--------------------------------------------------------------------------------------
// AE Global Variables
//--------------------------------------------------------------------------------------
AEResourceTable								ae_ResourceTable(100);
AEConstantTable<AEObject>					ae_ObjectTable(100);
AEHashedTable<AEAI>							ae_AITable(100);
AEHashedTable<AEParticleEmitter>			ae_ParticleTable(1000);
AEBackgroundLibrary							ae_BGLibrary;
AESceneManager								ae_SceneManager;
AECamera									ae_Camera;


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void AENSCore::CleanupDevice() {

	if (g_pImmediateContext) g_pImmediateContext->ClearState();
	if (g_pSamplerLinear) g_pSamplerLinear->Release();
	if (g_pCBNeverChanges) g_pCBNeverChanges->Release();
	if (g_pCBChangeOnResize) g_pCBChangeOnResize->Release();
	if (g_pCBChangesEveryFrame) g_pCBChangesEveryFrame->Release();
	if (g_pVertexBuffer) g_pVertexBuffer->Release();
	if (g_pIndexBuffer) g_pIndexBuffer->Release();
	if (g_pVertexLayout) g_pVertexLayout->Release();
	if (g_pVertexShader) g_pVertexShader->Release();
	if (g_pPixelShader) g_pPixelShader->Release();
	if (g_pBlendState) g_pBlendState->Release();
	if (g_pRasterizerState) g_pRasterizerState->Release();
	if (g_pDepthStencil) g_pDepthStencil->Release();
	if (g_pDepthStencilView) g_pDepthStencilView->Release();
	if (g_pRenderTargetView) g_pRenderTargetView->Release();
	if (g_pSwapChain) g_pSwapChain->Release();
	if (g_pImmediateContext1) g_pImmediateContext1->Release();
	if (g_pImmediateContext) g_pImmediateContext->Release();
	if (g_pd3dDevice1) g_pd3dDevice1->Release();
	if (g_pd3dDevice) g_pd3dDevice->Release();

	g_pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	if (g_pDebug) g_pDebug->Release();

}