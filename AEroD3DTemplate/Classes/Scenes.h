#pragma once

class TemplateScene: public AEScene {

public:

	TemplateScene(AEBackground* _bg, AEHashedTable<AESprite>* _spriteTable, AEHeadUpDisplay* _hud);

	VOID update();
	VOID paint();
	VOID processInput(CHAR* pKeyStateBuffer);

private:



};