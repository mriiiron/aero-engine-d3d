#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <dinput.h>
#include <d3d11sdklayers.h>
#include <time.h>

#include "AEroEngine.h"

using namespace DirectX;

//--------------------------------------------------------------------------------------
// D3D11 Global Variables
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

XMMATRIX									gm_World;
XMMATRIX									gm_View;
XMMATRIX									gm_Projection;
XMMATRIX									gm_Transform;
XMMATRIX									gm_TransformForHUD;

INT											gi_WindowWidth;
INT											gi_WindowHeight;

//--------------------------------------------------------------------------------------
// AE Global Variables
//--------------------------------------------------------------------------------------
AEConstantTable<AEResource>					ae_ResourceTable(100);
AEConstantTable<AEObject>					ae_ObjectTable(100);
//AEHashedTable<AEParticleEmitter>			ae_ParticleTable(1000);
AEBackgroundLibrary							ae_BGLibrary;
AESceneManager								ae_SceneManager;
AECamera									ae_Camera;

//--------------------------------------------------------------------------------------
// DirectXTK Global Variables
//--------------------------------------------------------------------------------------
SpriteBatch*								xtk_SpriteBatch;
SpriteFont*									xtk_SpriteFont_Arial_10;

//--------------------------------------------------------------------------------------
// Initialize
//--------------------------------------------------------------------------------------
VOID AENSCore::AEInitialize() {
    srand(time(nullptr));
}

//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
VOID AENSCore::CleanupDevice() {

    if (xtk_SpriteFont_Arial_10) delete xtk_SpriteFont_Arial_10;
    if (xtk_SpriteBatch) delete xtk_SpriteBatch;
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

//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DCompile
//
// With VS 11, we could load up prebuilt .cso files instead...
//--------------------------------------------------------------------------------------
HRESULT AENSCore::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
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
    hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
        dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
    if (FAILED(hr))
    {
        if (pErrorBlob)
        {
            OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
            pErrorBlob->Release();
        }
        return hr;
    }
    if (pErrorBlob) pErrorBlob->Release();

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT AENSCore::InitDevice()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect(g_hWnd, &rc);
    gi_WindowWidth = rc.right - rc.left;
    gi_WindowHeight = rc.bottom - rc.top;

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
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    // Create device and swap Chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = gi_WindowWidth;
    sd.BufferDesc.Height = gi_WindowHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        g_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain(nullptr, g_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
            D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);

        if (hr == E_INVALIDARG)
        {
            // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
            hr = D3D11CreateDeviceAndSwapChain(nullptr, g_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
                D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
        }

        if (SUCCEEDED(hr))
            break;
    }
    if (FAILED(hr))
        return hr;

    // Enable debug layer
    hr = g_pd3dDevice->QueryInterface(IID_PPV_ARGS(&g_pDebug));
    if (FAILED(hr))
        return hr;

    // Obtain the Direct3D 11.1 versions if available
    hr = g_pd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&g_pd3dDevice1));
    if (SUCCEEDED(hr))
    {
        (void)g_pImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&g_pImmediateContext1));
    }

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(hr))
        return hr;

    hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr))
        return hr;

    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory(&descDepth, sizeof(descDepth));
    descDepth.Width = gi_WindowWidth;
    descDepth.Height = gi_WindowHeight;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = g_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &g_pDepthStencil);
    if (FAILED(hr))
        return hr;

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory(&descDSV, sizeof(descDSV));
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
    if (FAILED(hr))
        return hr;

    g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)gi_WindowWidth;
    vp.Height = (FLOAT)gi_WindowHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_pImmediateContext->RSSetViewports(1, &vp);

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
    hr = g_pd3dDevice->CreateBlendState(&descBlend, &g_pBlendState);
    if (FAILED(hr))
    {
        MessageBox(nullptr,
            "Cannot create blend state.", "Error", MB_OK);
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
    hr = g_pd3dDevice->CreateRasterizerState(&descRasterizer, &g_pRasterizerState);
    if (FAILED(hr))
    {
        MessageBox(nullptr,
            "Cannot create rasterizer state.", "Error", MB_OK);
        return hr;
    }
    g_pImmediateContext->RSSetState(g_pRasterizerState);

    // Compile the vertex shader
    ID3DBlob* pVSBlob = nullptr;
    hr = CompileShaderFromFile(L"AEroD3D.fx", "VS", "vs_4_0", &pVSBlob);
    if (FAILED(hr))
    {
        MessageBox(nullptr,
            "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
        return hr;
    }

    // Create the vertex shader
    hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader);
    if (FAILED(hr))
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
    UINT numElements = ARRAYSIZE(layout);

    // Create the vertex layouts
    hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pVertexLayout);
    pVSBlob->Release();
    if (FAILED(hr))
        return hr;

    // Set the vertex layout
    g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

    // Compile the pixel shader
    ID3DBlob* pPSBlob = nullptr;
    hr = CompileShaderFromFile(L"AEroD3D.fx", "PS", "ps_4_0", &pPSBlob);
    if (FAILED(hr))
    {
        MessageBox(nullptr,
            "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
        return hr;
    }

    // Create the pixel shader
    hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader);
    pPSBlob->Release();
    if (FAILED(hr))
        return hr;

    // Create vertex buffer as dynamic (CPU write only / GPU read only)
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(SimpleVertex) * 1000;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pVertexBuffer);
    if (FAILED(hr)) {
        return hr;
    }

    // Set vertex buffer
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // Create index buffer as dynamic (CPU write only / GPU read only)
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(WORD) * 1500;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pIndexBuffer);
    if (FAILED(hr)) {
        return hr;
    }

    // Set index buffer
    g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    // Set primitive topology
    g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Create the sampler state
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);
    if (FAILED(hr))
        return hr;

    // Initialize the world matrix
    gm_World = XMMatrixIdentity();

    // Initialize the view matrix
    XMVECTOR Eye = XMVectorSet(-(gi_WindowWidth / 2.0f), -(gi_WindowHeight / 2.0f), 0.0f, 0.0f);
    XMVECTOR At = XMVectorSet(-(gi_WindowWidth / 2.0f), -(gi_WindowHeight / 2.0f), 100.0f, 0.0f);
    XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    gm_View = XMMatrixLookAtLH(Eye, At, Up);
    // gm_View = XMMatrixIdentity();

    // Initialize the projection matrix
    gm_Projection = XMMatrixIdentity();

    // Initialize transform matrices
    gm_TransformForHUD = gm_World * gm_View * gm_Projection;
    gm_Transform = gm_World * gm_View * gm_Projection;

    xtk_SpriteBatch = new SpriteBatch(g_pImmediateContext);
    xtk_SpriteFont_Arial_10 = new SpriteFont(g_pd3dDevice, L"Resources\\arial_10.spritefont");

    return S_OK;
}