/**
 * @file	AEScene.h
 * @brief	Defining the game scenes.
 *
 * The scenes are almost the most important components in a game created using AEro Engine.
 * Each scene could own its own background, sprite table and HUD. Meanwhile different scenes
 * can react differently to the same input. For example, when you click somewhere in an "menu"
 * scene, a menu item might be selected; but if you click on your enemy in the "gameplay" scene,
 * your hero would just start to shoot.
 *
 * @author	CaiyZ (Mr.IroN)
 * @version	0.1
 * @date	2013
*/

#pragma once


class AEScene {

public:

	AEScene(AEBackground* _bg, AEHashedTable<AESprite>* _spriteTable, AEHeadUpDisplay* _hud);
	~AEScene();

	CHAR keyStateBuffer[256];

	AEBackground* getBackground() { return bg; }
	AEHashedTable<AESprite>* getSpriteTable() { return spriteTable; }
	AEHeadUpDisplay* getHUD() { return hud; }

	VOID addSprite(AESprite* sprite);
	virtual VOID update();
	virtual VOID render();
	virtual VOID processInput();

	BOOLEAN textDebug;

protected:

	AEBackground* bg;
	AEHashedTable<AESprite>* spriteTable;
	AEHeadUpDisplay* hud;

};


class AESceneManager {

public:

	static const INT MAX_SCENE_COUNT			= 20;
	static const INT SCENE_NONE					= -1;

	AESceneManager();

	AEScene* getActiveScene() { return activeSceneIndex >= 0 ? table[activeSceneIndex] : nullptr; }
	
	VOID addSceneAt(INT index, AEScene* scene);
	VOID stopAll();
	VOID runScene(INT index);
	

private:

	AEScene* table[MAX_SCENE_COUNT];
	INT activeSceneIndex;

};