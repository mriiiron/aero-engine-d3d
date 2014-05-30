#include <dinput.h>

#include "AEroEngine.h"
#include "Scenes.h"

extern AESceneManager						ae_SceneManager;

GeneralScene::GeneralScene(AEBackground* _bg, AEHashedTable<AESprite>* _spriteTable, AEHeadUpDisplay* _hud) : AEScene(_bg, _spriteTable, _hud) {
	// ^^
}

VOID GeneralScene::update() {
	AEScene::update();
}

VOID GeneralScene::render() {
	AEScene::render();
}

VOID GeneralScene::processInput() {
	if (keyStateBuffer[DIK_A] & 0x80) {
		AESprite* pGedama = ae_SceneManager.getActiveScene()->getSpriteTable()->getItemByHash(0);
		pGedama->adjustAlpha(-0.005f);
	}
	else if (keyStateBuffer[DIK_S] & 0x80) {
		AESprite* pGedama = ae_SceneManager.getActiveScene()->getSpriteTable()->getItemByHash(0);
		pGedama->adjustAlpha(0.005f);
	}
}