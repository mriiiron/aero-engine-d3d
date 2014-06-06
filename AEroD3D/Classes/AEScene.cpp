#include <d3d11_1.h>
#include "AEMath.h"
#include "AETable.h"
#include "AEResource.h"
#include "AEBackground.h"
#include "AEHeadUpDisplay.h"
#include "AESprite.h"
#include "AECamera.h"
#include "AEScene.h"

extern AECamera									ae_Camera;
extern AEResourceTable							ae_ResourceTable;

AEScene::AEScene(AEBackground* _bg, AEHashedTable<AESprite>* _spriteTable, AEHeadUpDisplay* _hud) {
	bg = _bg;
	spriteTable = _spriteTable;
	hud = _hud;
	for (INT i = 0; i < 256; i++) {
		keyStateBuffer[i] = 0;
	}
}

AEScene::~AEScene() {
	if (bg) delete bg;
	if (spriteTable) delete spriteTable;
	if (hud) delete hud;
}

VOID AEScene::addSprite(AESprite* sprite) {
	sprite->setScene(this);
	spriteTable->add(sprite);
}

VOID AEScene::update() {
	processInput();
	if (bg) {
		bg->update();
	}
	if (spriteTable) {
		for (INT iHash = 0; iHash < spriteTable->getHashCount(); iHash++) {
			spriteTable->getItemByHash(iHash)->update();
		}
	}
	if (hud) {
		hud->update();
	}
}

VOID AEScene::render() {
	if (bg) {
		bg->addToRenderBuffer(ae_Camera.getFocus());
		ae_ResourceTable.renderAndClear();
	}
	if (spriteTable) {
		FLOAT zBias = 0.0f;
		for (INT iHash = 0; iHash < spriteTable->getHashCount(); iHash++) {
			spriteTable->getItemByHash(iHash)->addToRenderBuffer(zBias);
			zBias -= 0.065f;
		}
		ae_ResourceTable.renderAndClear();
	}
	if (hud) {
		hud->addToRenderBuffer();
		ae_ResourceTable.renderAndClear();
	}
}

VOID AEScene::processInput() {
	// TODO
}


AESceneManager::AESceneManager() {
	for (INT i = 0; i < MAX_SCENE_COUNT; i++) {
		table[i] = NULL;
	}
	activeSceneIndex = SCENE_NONE;
}

VOID AESceneManager::addSceneAt(INT index, AEScene* scene) {
	if (index < 0 || index > MAX_SCENE_COUNT || scene == NULL) {
		// Error
		return;
	}
	if (table[index] != NULL) {
		// Error
		return;
	}
	table[index] = scene;
}

VOID AESceneManager::stopAll() {
	activeSceneIndex = SCENE_NONE;
}

VOID AESceneManager::runScene(INT index) {
	activeSceneIndex = index;
}