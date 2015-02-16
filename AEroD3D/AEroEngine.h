#pragma once

// DirectXTK Classes

#include "Classes\XTK\DDSTextureLoader.h"
#include "Classes\XTK\SpriteBatch.h"
#include "Classes\XTK\SpriteFont.h"
#include "Classes\XTK\CommonStates.h"


// AEro Engine Classes

#include "Classes\AEMath.h"
#include "Classes\AEPhysics.h"
#include "Classes\AECollision.h"

#include "Classes\AEGameControl.h"

#include "Classes\AETable.h"
#include "Classes\AEResource.h"
#include "Classes\AEBackground.h"
#include "Classes\AEPlatform.h"
#include "Classes\AEParticleSystem.h"

#include "Classes\AEObject.h"

#include "Classes\AESprite.h"
#include "Classes\AEHeadUpDisplay.h"
#include "Classes\AECamera.h"

#include "Classes\AEScene.h"
#include "Classes\AEAI.h"


// Core Namespace

namespace AENSCore {

    VOID AEInitialize();
    VOID CleanupDevice();

    HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
    HRESULT InitDevice();

    LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
    void LoadGameResources();
    void InitGameplay();
    void Update();
    void Render();

}