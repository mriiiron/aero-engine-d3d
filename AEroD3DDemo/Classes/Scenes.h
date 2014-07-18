#pragma once


// General
class GeneralSampleScene: public AEScene {

public:

	GeneralSampleScene(AEBackground* _bg, AEHashedTable<AEPlatform>* _platformTable, AEHashedTable<AESprite>* _spriteTable, AEHeadUpDisplay* _hud);

	VOID initialize();
	VOID update();
	VOID render();
	VOID processInput();

private:



};


// Vertical Scroller
class VerticalScrollerScene : public AEScene {

public:

	VerticalScrollerScene(AEBackground* _bg, AEHashedTable<AEPlatform>* _platformTable, AEHashedTable<AESprite>* _spriteTable, AEHeadUpDisplay* _hud);
	~VerticalScrollerScene();

	VOID initialize();
	VOID update();
	VOID render();
	VOID processInput();

	VOID clipOutFarSprites();
	VOID restrictPlayerScope();

	AESprite* getPlayer() { return player; }

private:

	JFighterSprite* player;
	
	INT borderLeft = -320;
	INT borderRight = 320;
	INT borderBottom = 240;
	INT borderTop = -240;

};


// Side Scroller with Platform
class SideScrollerPlatformScene : public AEScene {

public:

	SideScrollerPlatformScene(AEBackground* _bg, AEHashedTable<AEPlatform>* _platformTable, AEHashedTable<AESprite>* _spriteTable, AEHeadUpDisplay* _hud);
	~SideScrollerPlatformScene();

	VOID initialize();
	VOID update();
	VOID render();
	VOID processInput();

private:

	Helicopter* player;

};