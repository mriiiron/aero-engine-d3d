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

	AEScene(AEBackground* _bg, AEHashedTable<AEPlatform>* _platformTable, AEHashedTable<AESprite>* _spriteTable, AEHeadUpDisplay* _hud);
	AEScene(INT spriteTableSize);
	~AEScene();

	CHAR keyStateBuffer[256];

	AEBackground* getBackground() { return bg; }
	AEHashedTable<AEPlatform>* getplatformTable() { return platformTable; }
	AEHashedTable<AESprite>* getSpriteTable() { return spriteTable; }
	AEHeadUpDisplay* getHUD() { return hud; }
	VOID setBackground(AEBackground* _bg) { bg = _bg; }
	VOID setPlatformTable(AEHashedTable<AEPlatform>* _platformTable) { platformTable = _platformTable; }
	VOID setSpriteTable(AEHashedTable<AESprite>* _spriteTable) { spriteTable = _spriteTable; }
	VOID setHUD(AEHeadUpDisplay* _hud) { hud = _hud; }

	VOID addSprite(AESprite* sprite);
	VOID addSpriteAttachment(AESprite* host, AESprite* attachment);

	VOID togglePause() { isPaused = !isPaused; }

	virtual VOID update();
	virtual VOID render();
	virtual VOID processInput();

protected:

	AEBackground* bg;
	AEHashedTable<AEPlatform>* platformTable;
	AEHashedTable<AESprite>* spriteTable;
	AEHeadUpDisplay* hud;

	BOOLEAN isPauseKeyPressed, isPaused;

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