#include "AEroEngine.h"
#include "Scenes.h"

TemplateScene::TemplateScene(AEBackground* _bg, AEHashedTable<AESprite>* _spriteTable, AEHeadUpDisplay* _hud) : AEScene(_bg, _spriteTable, _hud) {
	// ^^
}

VOID TemplateScene::update() {
	AEScene::update();
}

VOID TemplateScene::render() {
	AEScene::render();
}

VOID TemplateScene::processInput(CHAR* pKeyStateBuffer) {
	AEScene::processInput(pKeyStateBuffer);
}