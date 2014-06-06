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


// Vertical Scroller
class VerticalScrollerScene : public AEScene {

public:

	VerticalScrollerScene(AEBackground* _bg, AEHashedTable<AESprite>* _spriteTable, AEHeadUpDisplay* _hud);

	VOID initialize();
	VOID update();
	VOID render();
	VOID processInput();

	VOID clipOutFarSprites();

private:

	JFighterSprite* player;

};