#pragma once


// General
class GeneralScene: public AEScene {

public:

	GeneralScene(AEBackground* _bg, AEHashedTable<AESprite>* _spriteTable, AEHeadUpDisplay* _hud);

	VOID update();
	VOID render();
	VOID processInput();

private:



};