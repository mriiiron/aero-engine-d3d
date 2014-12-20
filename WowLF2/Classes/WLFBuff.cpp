#include "AEroEngine.h"
#include "WLFBuff.h"
#include "WLFSprite.h"

extern AEConstantTable<AEObject>			ae_ObjectTable;


WLFBuff::WLFBuff(INT _time) {
	host = nullptr;
	name = "Unknown";
	timeRemain = _time;
	iconIndex = 0;
	deadFlag = FALSE;
}

AESprite* WLFBuff::getSprite(FLOAT cx, FLOAT cy) {
	AERO_SPRITE_DESC descSpr;
	descSpr.obj = ae_ObjectTable.getItem(11);
	descSpr.team = -1;
	descSpr.action = iconIndex;
	descSpr.flip = SpriteEffects_None;
	descSpr.cx = cx;
	descSpr.cy = cy;
	descSpr.layerDepth = 0.01f;
	return new AESprite(descSpr);
}


WLFBuffRend::WLFBuffRend(INT _time) : WLFBuff(_time) {
	name = "Rend";
	iconIndex = 1;
}

VOID WLFBuffRend::update() {
	if (timeRemain % 120 == 0) {
		host->takeDamage(1);
	}
	timeRemain--;
	if (timeRemain <= 0) {
		deadFlag = TRUE;
	}
}


WLFBuffArmorCrack::WLFBuffArmorCrack(INT _time) : WLFBuff(_time) {
	name = "Armor Crack";
	iconIndex = 2;
}

VOID WLFBuffArmorCrack::update() {
	
	timeRemain--;
	if (timeRemain <= 0) {
		deadFlag = TRUE;
	}
}


WLFBuffSlowDown::WLFBuffSlowDown(INT _time) : WLFBuff(_time) {
	name = "Slow Down";
	iconIndex = 3;
}

VOID WLFBuffSlowDown::update() {

	timeRemain--;
	if (timeRemain <= 0) {
		deadFlag = TRUE;
	}
}