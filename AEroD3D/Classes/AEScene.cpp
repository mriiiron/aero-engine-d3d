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
	hud->setScene(this);
	for (INT i = 0; i < 256; i++) {
		keyStateBuffer[i] = 0;
	}
	isPauseKeyPressed = isPaused = FALSE;
	standstill = 0;
}

AEScene::AEScene(INT spriteTableSize) {
	bg = nullptr;
	platformTable = nullptr;
	spriteTable = new AEHashedTable<AESprite>(spriteTableSize);
	hud = nullptr;
	for (INT i = 0; i < 256; i++) {
		keyStateBuffer[i] = 0;
	}
	isPauseKeyPressed = isPaused = FALSE;
}

AEScene::~AEScene() {
	if (bg) delete bg;
	if (platformTable) delete platformTable;
	if (spriteTable) delete spriteTable;
	if (hud) delete hud;
}

VOID AEScene::addSprite(AESprite* sprite) {
	sprite->setScene(this);
	sprite->setIndex(spriteTable->add(sprite));
}

VOID AEScene::addSpriteAttachment(AESprite* host, AESprite* attachment) {
	if (!(host->hasAttachments())) {
		AENSGameControl::exitGame("Cannot add attachment for Sprite " + host->getObjName() + ".");
		return;
	}
	host->getAttachmentTable()->add(attachment);
	addSprite(attachment);
}

VOID AEScene::addSpriteForHUD(AESprite* hudSprite) {
	hudSprite->setScene(this);
	hudSprite->setIndex(hud->getSpriteTable()->add(hudSprite));
}

VOID AEScene::update() {
	if (isPaused) {
		return;
	}
	if (standstill > 0) {
		standstill--;
		return;
	}
	processCollision();
	if (bg) {
		bg->update();
	}
	if (spriteTable) {

		// Update sprite table first
		// Removing a sprite directly from sprite table is forbidden
		// Must set a sprite to death (deadFlag = true), then remove it next time we update the scene
		for (INT iHash = 0; iHash < spriteTable->getHashCount(); iHash++) {
			AESprite* sprite = spriteTable->getItemByHash(iHash);
			if (sprite->isDead()) {
				spriteTable->removeItemByHash(iHash);
			}
			else {
				sprite->update(platformTable);
			}
		}

		// Update attachment table for each sprite after updating sprite table
		// If any attachment sprite is set to death (deadFlag == true), clear the corresponding pointer in attachment table here
		// To avoid memory leak
		for (INT iHash = 0; iHash < spriteTable->getHashCount(); iHash++) {
			AESprite* sprite = spriteTable->getItemByHash(iHash);
			if (sprite->hasAttachments()) {
				sprite->updateAttachments();
			}
		}
			
	}
	if (hud) {
		hud->update();
	}

}

VOID AEScene::render(INT renderMode) {
	if (bg && (renderMode & RENDER_BACKGROUND)) {
		bg->render(ae_Camera.getFocus());
	}
	if (spriteTable && (renderMode & RENDER_SPRITES)) {
		for (INT iHash = 0; iHash < spriteTable->getHashCount(); iHash++) {
			AESprite* sprite = spriteTable->getItemByHash(iHash);
			if (!(sprite->isDead())) {
				sprite->render();
			}
		}
	}
	if (hud && (renderMode & RENDER_HUD)) {
		hud->render();
	}
}

VOID AEScene::processInput() {
	// To be inherited
}

VOID AEScene::processCollision() {
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