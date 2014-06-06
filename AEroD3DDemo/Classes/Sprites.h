#pragma once


class JFighterSprite : public AESprite {

public:

	JFighterSprite(AERO_SPRITE_DESC desc);

	VOID update();

	VOID shoot();

	BOOLEAN isFireKeyPressed, isRocketCoolingDown;

private:

	INT currentRocketSlotIndex;
	INT rocketSlotOffset[6];
	INT coolingDownTimer;

};