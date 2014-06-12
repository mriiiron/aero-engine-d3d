#include "AEroEngine.h"
#include "AI.h"
#include "Sprites.h"

extern AEConstantTable<AEObject>			ae_ObjectTable;

JFighterSprite::JFighterSprite(AERO_SPRITE_DESC desc) : AESprite(desc) {
	currentRocketSlotIndex = 0;
	rocketSlotOffset[0] = -15;  rocketSlotOffset[1] = 15;
	rocketSlotOffset[2] = -12;  rocketSlotOffset[3] = 12;
	rocketSlotOffset[4] = -9;  rocketSlotOffset[5] = 9;
	isFireKeyPressed = FALSE;
	isRocketCoolingDown = FALSE;
	coolingDownTimer = 0;
}

VOID JFighterSprite::update() {
	AESprite::update();
	if (isRocketCoolingDown) {
		coolingDownTimer--;
		if (coolingDownTimer <= 0) {
			isRocketCoolingDown = FALSE;
			currentRocketSlotIndex = 0;
		}
	}
}

VOID JFighterSprite::shoot() {
	if (isRocketCoolingDown) {
		return;
	}
	AERO_SPRITE_DESC descSpr;
	descSpr.obj = ae_ObjectTable.getItem(2);
	descSpr.team = 0;
	descSpr.action = 0;
	descSpr.facing = 0;
	descSpr.cx = cx + rocketSlotOffset[currentRocketSlotIndex];
	descSpr.cy = cy;
	AESprite* rocket = new AESprite(descSpr);
	rocket->setAI(new RocketAI(rocket));
	scene->addSprite(rocket);
	currentRocketSlotIndex++;
	if (currentRocketSlotIndex >= 6) {
		isRocketCoolingDown = TRUE;
		coolingDownTimer = 300;
	}
}