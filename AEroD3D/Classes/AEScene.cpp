#include <d3d11_1.h>
#include "AEMath.h"
#include "AETable.h"
#include "AEResource.h"
#include "AEBackground.h"
#include "AEHeadUpDisplay.h"
#include "AESprite.h"
#include "AECamera.h"
#include "AEScene.h"

extern AECamera									camera;
extern AEResourceTable							resourceTable;

AEScene::AEScene(AEBackground* _bg, AEHashedTable<AESprite>* _spriteTable, AEHeadUpDisplay* _hud) {
	bg = _bg;
	spriteTable = _spriteTable;
	hud = _hud;
}

AEScene::~AEScene() {
	delete bg;
	delete spriteTable;
	delete hud;
}

VOID AEScene::addSprite(AESprite* sprite) {
	spriteTable->add(sprite);
}

VOID AEScene::update() {
	bg->update();
	for (INT iHash = 0; iHash < spriteTable->getHashCount(); iHash++) {
		spriteTable->getItemByHash(iHash)->update();
	}
	hud->update();
}

VOID AEScene::render() {
	bg->addToRenderBuffer(camera.getFocus());
	resourceTable.renderAndClear();
	FLOAT zBias = 0.0f;
	for (INT iHash = 0; iHash < spriteTable->getHashCount(); iHash++) {
		spriteTable->getItemByHash(iHash)->addToRenderBuffer(zBias);
		zBias -= 0.065f;
	}
	resourceTable.renderAndClear();
	hud->addToRenderBuffer();
	resourceTable.renderAndClear();
}

VOID AEScene::processInput(CHAR* pKeyStateBuffer) {
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