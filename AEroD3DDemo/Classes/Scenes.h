#pragma once


// General
class GeneralSampleScene: public AEScene {

public:

	GeneralSampleScene(AEBackground* _bg, AEHashedTable<AESprite>* _spriteTable, AEHeadUpDisplay* _hud);

	VOID initialize();
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