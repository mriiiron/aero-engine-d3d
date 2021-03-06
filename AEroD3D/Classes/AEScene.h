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


struct AERO_SCENE_DESC {

    INT spriteTableSize;
    AEBackground* bg;
    AEHeadUpDisplay* hud;

    AERO_SCENE_DESC() {
        spriteTableSize = 0;
        bg = nullptr;
        hud = nullptr;
    }

};

class AEScene {

public:

    const static INT RENDER_ALL = 0x7;
    const static INT RENDER_BACKGROUND = 0x1;
    const static INT RENDER_SPRITES = 0x2;
    const static INT RENDER_HUD = 0x4;

    AEScene(AERO_SCENE_DESC desc);
    ~AEScene();

    CHAR keyStateBuffer[256];

    AEBackground* getBackground() { return bg; }
    AEHashedTable<AESprite>* getSpriteTable() { return spriteTable; }
    AEHeadUpDisplay* getHUD() { return hud; }
    INT getStandstill() { return standstill; }
    VOID setBackground(AEBackground* _bg) { bg = _bg; }
    VOID setSpriteTable(AEHashedTable<AESprite>* _spriteTable) { spriteTable = _spriteTable; }
    VOID setHUD(AEHeadUpDisplay* _hud) { hud = _hud;  hud->setScene(this); }

    VOID addSprite(AESprite* sprite);
    VOID addSpriteAttachment(AESprite* host, AESprite* attachmentSprite, INT slot, INT attachMode = AESpriteAttachment::ATTACH_MODE_FOLLOW);
    VOID addSpriteForHUD(AESprite* hudSprite);

    VOID togglePause() { isPaused = !isPaused; }
    VOID setStandstill(INT time) { standstill = time; }

    virtual VOID update();
    virtual VOID render(INT renderMode = RENDER_ALL);
    virtual VOID processInput();
    virtual VOID processCollision();

protected:

    AEBackground* bg;
    AEHashedTable<AESprite>* spriteTable;
    AEHeadUpDisplay* hud;

    BOOLEAN isPauseKeyPressed, isStepFrameKeyPressed, isPaused, isStepFraming;

    INT standstill;

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