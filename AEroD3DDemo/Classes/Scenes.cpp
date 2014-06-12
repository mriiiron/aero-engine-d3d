#include <dinput.h>

#include "AEroEngine.h"
#include "Sprites.h"
#include "Scenes.h"

extern AESceneManager						ae_SceneManager;

GeneralScene::GeneralScene(AEBackground* _bg, AEHashedTable<AESprite>* _spriteTable, AEHeadUpDisplay* _hud) : AEScene(_bg, _spriteTable, _hud) {

}

VOID GeneralScene::update() {
	AEScene::update();
}

VOID GeneralScene::render() {
	AEScene::render();
}

VOID GeneralScene::processInput() {

	if (keyStateBuffer[DIK_2] & 0x80) {
		ae_SceneManager.runScene(1);
	}

	if (keyStateBuffer[DIK_A] & 0x80) {
		AESprite* pGedama = ae_SceneManager.getActiveScene()->getSpriteTable()->getItemByHash(0);
		pGedama->adjustAlpha(-0.005f);
	}
	if (keyStateBuffer[DIK_S] & 0x80) {
		AESprite* pGedama = ae_SceneManager.getActiveScene()->getSpriteTable()->getItemByHash(0);
		pGedama->adjustAlpha(0.005f);
	}
	if (keyStateBuffer[DIK_Q] & 0x80) {
		AESprite* pGedama = ae_SceneManager.getActiveScene()->getSpriteTable()->getItemByHash(0);
		pGedama->rotateDeg(1.0);
	}
	if (keyStateBuffer[DIK_W] & 0x80) {
		AESprite* pGedama = ae_SceneManager.getActiveScene()->getSpriteTable()->getItemByHash(0);
		pGedama->rotateDeg(-1.0);
	}

}


VerticalScrollerScene::VerticalScrollerScene(AEBackground* _bg, AEHashedTable<AESprite>* _spriteTable, AEHeadUpDisplay* _hud) : AEScene(_bg, _spriteTable, _hud) {
	
}

VOID VerticalScrollerScene::initialize() {
	player = (JFighterSprite*)(spriteTable->getItemByHash(0));
}

VOID VerticalScrollerScene::update() {
	AEScene::update();
}

VOID VerticalScrollerScene::render() {
	AEScene::render();
}

VOID VerticalScrollerScene::processInput() {
	
	if (keyStateBuffer[DIK_1] & 0x80) {
		ae_SceneManager.runScene(0);
	}

	if (keyStateBuffer[DIK_A] & 0x80) {
		player->move(-0.7f, 0.0f);
	}
	if (keyStateBuffer[DIK_D] & 0x80) {
		player->move(0.7f, 0.0f);
	}
	if (keyStateBuffer[DIK_W] & 0x80) {
		player->move(0.0f, 0.7f);
	}
	if (keyStateBuffer[DIK_S] & 0x80) {
		player->move(0.0f, -0.7f);
	}
	if (keyStateBuffer[DIK_J] & 0x80) {
		if (!player->isFireKeyPressed) {
			player->isFireKeyPressed = TRUE;
			player->shoot();
		}
	}
	else {
		player->isFireKeyPressed = FALSE;
	}

}

VOID VerticalScrollerScene::clipOutFarSprites() {
	// TODO
}