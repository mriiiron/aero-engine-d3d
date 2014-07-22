#include <d3d11_1.h>
#include "AEMath.h"
#include "AETable.h"
#include "AEResource.h"
#include "AEBackground.h"
#include "AEHeadUpDisplay.h"
#include "AEPlatform.h"
#include "AESprite.h"
#include "AECamera.h"
#include "AEScene.h"

extern AECamera								ae_Camera;
extern AEResourceTable						ae_ResourceTable;

AEScene::AEScene(AEBackground* _bg, AEHashedTable<AEPlatform>* _platformTable, AEHashedTable<AESprite>* _spriteTable, AEHeadUpDisplay* _hud) {
	bg = _bg;
	platformTable = _platformTable;
	spriteTable = _spriteTable;
	hud = _hud;
	for (INT i = 0; i < 256; i++) {
		keyStateBuffer[i] = 0;
	}
	isPauseKeyPressed = isPaused = FALSE;
}

AEScene::AEScene(INT spriteTableSize) {
	bg = nullptr;
	platformTable = nullptr;
	spriteTable = new AEHashedTable<AESprite>(spriteTableSize);
	hud = nullptr;
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

VOID AEScene::pause() {
	isPaused = !isPaused;
}

VOID AEScene::update() {
	if (!isPaused) {
		if (bg) {
			bg->update();
		}
		if (spriteTable) {
			for (INT iHash = 0; iHash < spriteTable->getHashCount(); iHash++) {
				AESprite* sprite = spriteTable->getItemByHash(iHash);
				if (sprite->isDead()) {
					spriteTable->remove(spriteTable->getHash(iHash));
				}
				else {
					sprite->update(platformTable);
				}
			}
		}
		if (hud) {
			hud->update();
		}
	}
}

VOID AEScene::render() {
	if (bg) {
		bg->render(ae_Camera.getFocus());
		//ae_ResourceTable.renderAndClear();
	}
	if (spriteTable) {
		for (INT iHash = 0; iHash < spriteTable->getHashCount(); iHash++) {
			AESprite* sprite = spriteTable->getItemByHash(iHash);
			if (!(sprite->isDead())) {
				sprite->render();
			}
		}
		//ae_ResourceTable.renderAndClear();
	}
	if (hud) {
		hud->addToRenderBuffer();
		//ae_ResourceTable.renderAndClear();
	}
}

VOID AEScene::processInput() {
	// To be inherited
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