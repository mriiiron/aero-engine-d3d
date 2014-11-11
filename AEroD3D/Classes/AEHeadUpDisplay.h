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


class AEHeadUpDisplay {

public:

	AEHeadUpDisplay(INT spriteTableSize);

	AEHashedTable<AESprite>* getSpriteTable() { return spriteTable; }

	VOID update();
	VOID render();

protected:

	AEHashedTable<AESprite>* spriteTable;

};