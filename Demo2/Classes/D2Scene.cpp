#include <dinput.h>

#include "AEroEngine.h"
#include "D2AI.h"
#include "D2Sprite.h"
#include "D2Scene.h"

extern AEConstantTable<AEResource>			ae_ResourceTable;
extern AEConstantTable<AEObject>			ae_ObjectTable;
extern AEConstantTable<AEBackground>        ae_BackgroundTable;
extern AESceneManager						ae_SceneManager;
extern AECamera                             ae_Camera;

const FLOAT D2Scene::GRAVITY = 0.1f;
const FLOAT D2Scene::FRACTION = 0.1f;

D2Scene::D2Scene(AERO_SCENE_DESC desc) : AEScene(desc) {
    dik_left = DIK_LEFT;
    dik_right = DIK_RIGHT;
    dik_jump = DIK_UP;
    standstill = 0;
}

VOID D2Scene::initialize() {

    // Create a wood man
    AERO_SPRITE_DESC descSpr;
    descSpr.obj = ae_ObjectTable.getItem(0);
    descSpr.team = 0;
    descSpr.action = 1;
    descSpr.flip = SpriteEffects_None;
    descSpr.cx = 0.0f;
    descSpr.cy = 0.0f;
    descSpr.layerDepth = 0.001f;
    D2Sprite* hellcow = new D2Sprite(descSpr);
    addSprite(hellcow);

    player = hellcow;

    // DEBUG
    player->setVx(-0.0f);
    player->setVy(0.5f);

}

VOID D2Scene::update() {
    AEScene::update();
    ae_Camera.setFocus(player->getCx(), 0.0f);
    ae_Camera.update();
}

VOID D2Scene::render(INT renderMode) {
    AEScene::render(renderMode);
}

VOID D2Scene::processInput() {
    if (!isPaused) {

        // Left Key (<)
        if (keyStateBuffer[dik_left] & 0x80) {
            player->keyDown(D2Keys::KEY_LEFT);
        }
        else {
            player->keyUp(D2Keys::KEY_LEFT);
        }

        // Right Key (>)
        if (keyStateBuffer[dik_right] & 0x80) {
            player->keyDown(D2Keys::KEY_RIGHT);
        }
        else {
            player->keyUp(D2Keys::KEY_RIGHT);
        }

        // Up Key (^)
        if (keyStateBuffer[dik_jump] & 0x80) {
            player->keyDown(D2Keys::KEY_JUMP);
        }
        else {
            player->keyUp(D2Keys::KEY_JUMP);
        }

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

VOID D2Scene::processCollision() {
    
}