/**
 * @file	AEHud.h
 * @brief	Defining the elements used by head-up displays.
 *
 * The HUDs is included in Scenes. Elements of HUD will not move along with ae_Camera movement.
 * You may create a relatively static scene only using HUD. For example, your game title screen.
 *
 * @author	CaiyZ (Mr.IroN)
 * @version	0.1
 * @date	2013
*/

#pragma once

#include "AESprite.h"


class AEScene;


class AEHeadUpDisplay {

public:

	static const INT NAMEPAD_SLOT_PLAYER = 0;
	static const INT NAMEPAD_SLOT_TARGET = 1;

	AEHeadUpDisplay(INT spriteTableSize);

	AEHashedTable<AESprite>* getSpriteTable() { return spriteTable; }
	VOID setScene(AEScene* _scene) { scene = _scene; }
	AEScene* getScene() { return scene; }

	virtual VOID update();
	virtual VOID render();

protected:

	AEScene* scene;
	AEHashedTable<AESprite>* spriteTable;

};