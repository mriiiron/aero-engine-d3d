#include <dinput.h>

#include "AEroEngine.h"
#include "AI.h"
#include "Sprite.h"
#include "Scene.h"

extern AEConstantTable<AEResource>			ae_ResourceTable;
extern AEConstantTable<AEObject>			ae_ObjectTable;
extern AEConstantTable<AEBackground>        ae_BackgroundTable;
extern AESceneManager						ae_SceneManager;
extern AECamera                             ae_Camera;

TemplateScene::TemplateScene(AERO_SCENE_DESC desc) : AEScene(desc) {
    standstill = 0;
}

VOID TemplateScene::initialize() {

    // Create a wood man
    AERO_SPRITE_DESC descSpr;
    descSpr.obj = ae_ObjectTable.getItem(0);
    descSpr.team = 0;
    descSpr.action = 0;
    descSpr.flip = SpriteEffects_None;
    descSpr.cx = 0.0f;
    descSpr.cy = 0.0f;
    descSpr.layerDepth = 0.001f;
    TemplateSprite* hellcow = new TemplateSprite(descSpr);
    addSprite(hellcow);

    // Set Camera Focus to Center
    ae_Camera.setFocus(0.0f, 0.0f);

}

VOID TemplateScene::update() {
    AEScene::update();
}

VOID TemplateScene::render(INT renderMode) {
    AEScene::render(renderMode);
}

VOID TemplateScene::processInput() {
    if (!isPaused) {

        // Do Stuff

    }
    if (keyStateBuffer[DIK_BACKSLASH] & 0x80) {
        if (!isStepFrameKeyPressed) {
            isStepFrameKeyPressed = TRUE;
            isStepFraming = TRUE;
            togglePause();
        }
    }
    else {
        isStepFrameKeyPressed = FALSE;
    }
    if (keyStateBuffer[DIK_RETURN] & 0x80) {
        if (!isPauseKeyPressed) {
            isPauseKeyPressed = TRUE;
            togglePause();
        }
    }
    else {
        isPauseKeyPressed = FALSE;
    }
}

VOID TemplateScene::processCollision() {
    
}