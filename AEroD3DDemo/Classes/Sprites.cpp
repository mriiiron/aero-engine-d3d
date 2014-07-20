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

VOID JFighterSprite::update(AEHashedTable<AEPlatform>* platformTable) {
	AESprite::update(platformTable);
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
	descSpr.flip = SpriteEffects_None;
	descSpr.cx = cx + rocketSlotOffset[currentRocketSlotIndex];
	descSpr.cy = cy;
	descSpr.layerDepth = 0.5f;
	Bullet* rocket = new Bullet(descSpr);
	rocket->rotateDeg(-90.0f);
	rocket->setAI(new RocketAI(rocket));
	scene->addSprite(rocket);
	currentRocketSlotIndex++;
	if (currentRocketSlotIndex >= 6) {
		isRocketCoolingDown = TRUE;
		coolingDownTimer = 300;
	}
}


Turret::Turret(AERO_SPRITE_DESC desc) : AESprite(desc) {
	firingSlot = 1;
}

VOID Turret::shootRocket() {
	XMFLOAT2 muzzle = AENSMath::rotatePoint(XMFLOAT2(cx + 15.0f, cy + (FLOAT)firingSlot * 10.0f), XMFLOAT2(cx, cy), angleDisplay);
	AERO_SPRITE_DESC descSpr;
	descSpr.obj = ae_ObjectTable.getItem(2);
	descSpr.team = 1;
	descSpr.action = 0;
	descSpr.flip = SpriteEffects_None;
	descSpr.cx = muzzle.x;
	descSpr.cy = muzzle.y;
	descSpr.layerDepth = 0.5f;
	Bullet* rocket = new Bullet(descSpr);
	rocket->rotateRad(angleDisplay);
	rocket->setAI(new RocketAI(rocket));
	scene->addSprite(rocket);
	firingSlot = -1 * firingSlot;
}

VOID Turret::shootHomingBullets() {
	AERO_SPRITE_DESC descSpr;
	descSpr.obj = ae_ObjectTable.getItem(6);
	descSpr.team = 1;
	descSpr.action = 0;
	descSpr.flip = SpriteEffects_None;
	XMFLOAT2 muzzleRight = AENSMath::rotatePoint(XMFLOAT2(cx - 12.0f, cy + 10.0f), XMFLOAT2(cx, cy), angleDisplay);
	descSpr.cx = muzzleRight.x;
	descSpr.cy = muzzleRight.y;
	descSpr.layerDepth = 0.5f;

	FLOAT baseSpeed = 5.0f;

	Bullet* homing_0 = new Bullet(descSpr);
	homing_0->setSpeed(baseSpeed);
	homing_0->rotateRad(angleDisplay + AENSMath::deg2rad(135.0f));
	homing_0->setAI(new HomingAI(homing_0, ((TurretAI*)ai)->getTarget()));

	Bullet* homing_1 = new Bullet(descSpr);
	homing_1->setSpeed(baseSpeed);
	homing_1->rotateRad(angleDisplay + AENSMath::deg2rad(90.0f));
	homing_1->setAI(new HomingAI(homing_1, ((TurretAI*)ai)->getTarget()));

	XMFLOAT2 muzzleLeft = AENSMath::rotatePoint(XMFLOAT2(cx - 12.0f, cy - 10.0f), XMFLOAT2(cx, cy), angleDisplay);
	descSpr.cx = muzzleLeft.x;
	descSpr.cy = muzzleLeft.y;

	Bullet* homing_2 = new Bullet(descSpr);
	homing_2->setSpeed(baseSpeed);
	homing_2->rotateRad(angleDisplay - AENSMath::deg2rad(135.0f));
	homing_2->setAI(new HomingAI(homing_2, ((TurretAI*)ai)->getTarget()));

	Bullet* homing_3 = new Bullet(descSpr);
	homing_3->setSpeed(baseSpeed);
	homing_3->rotateRad(angleDisplay - AENSMath::deg2rad(90.0f));
	homing_3->setAI(new HomingAI(homing_3, ((TurretAI*)ai)->getTarget()));

	scene->addSprite(homing_0);
	scene->addSprite(homing_1);
	scene->addSprite(homing_2);
	scene->addSprite(homing_3);
}


Helicopter::Helicopter(AERO_SPRITE_DESC desc) : AESprite(desc) {
	isFireKeyPressed = FALSE;
	isRocketCoolingDown = FALSE;
	coolingDownTimer = 0;
	pitchAngleBalanced = AENSMath::deg2rad(10.0f);
	pitchAngleMax = AENSMath::deg2rad(20.0f);
	pitchAngleMin = AENSMath::deg2rad(-15.0f);
	pitchAngleRestoreSpeed = AENSMath::deg2rad(0.2f);
}

VOID Helicopter::platformCollision(AEHashedTable<AEPlatform>* platformTable, AECollisionResult collisionResult) {
	AESprite::platformCollision(platformTable, collisionResult);
}

VOID Helicopter::update(AEHashedTable<AEPlatform>* platformTable) {
	AESprite::update(platformTable);
	if (angleDisplay > pitchAngleMax) {
		angleDisplay = pitchAngleMax;
	}	
	if (angleDisplay < pitchAngleMin) {
		angleDisplay = pitchAngleMin;
	}
	if (angleDisplay > pitchAngleBalanced) {
		angleDisplay = (angleDisplay - pitchAngleRestoreSpeed < pitchAngleBalanced) ? pitchAngleBalanced : angleDisplay - pitchAngleRestoreSpeed;
	}
	else if (angleDisplay < pitchAngleBalanced) {
		angleDisplay = (angleDisplay + pitchAngleRestoreSpeed > pitchAngleBalanced) ? pitchAngleBalanced : angleDisplay + pitchAngleRestoreSpeed;
	}
}

VOID Helicopter::shoot() {

}


FlakCannon::FlakCannon(AERO_SPRITE_DESC desc) : AESprite(desc) {

}

VOID FlakCannon::shoot() {
	XMFLOAT2 muzzle = AENSMath::rotatePoint(XMFLOAT2(cx + (flip ? -1 : 1) * 25.0f, cy), XMFLOAT2(cx, cy), angle);
	XMFLOAT2 randomAdjust = AENSMath::randomPointWithinCircle(2.0f);
	AERO_SPRITE_DESC descSpr;
	descSpr.obj = ae_ObjectTable.getItem(10);
	descSpr.team = 1;
	descSpr.action = 0;
	descSpr.flip = flip;
	descSpr.cx = muzzle.x + randomAdjust.x;
	descSpr.cy = muzzle.y + randomAdjust.y;
	descSpr.layerDepth = 0.0f;
	AESprite* muzzleFlash = new AESprite(descSpr);
	muzzleFlash->rotateRad(angle);
	scene->addSprite(muzzleFlash);
}

VOID FlakCannon::throwShell() {
	XMFLOAT2 ejectionport = AENSMath::rotatePoint(XMFLOAT2(cx + (flip ? -1 : 1) * 10.0f, cy), XMFLOAT2(cx, cy), angle);
	AERO_SPRITE_DESC descSpr;
	descSpr.obj = ae_ObjectTable.getItem(11);
	descSpr.team = 1;
	descSpr.action = 0;
	descSpr.flip = SpriteEffects_None;
	descSpr.cx = ejectionport.x;
	descSpr.cy = ejectionport.y;
	descSpr.layerDepth = 0.05f;
	Shell* shell = new Shell(descSpr);
	XMFLOAT2 speedAdjust = AENSMath::randomPointWithinCircle(2.0f);
	shell->setVx(1.0f + speedAdjust.x);
	shell->setVy(-4.0f + speedAdjust.y);
	shell->setAy(AEPhysics::GRAVITY);
	shell->setVAngleDeg(10.0f);
	scene->addSprite(shell);
}


Bullet::Bullet(AERO_SPRITE_DESC desc) : AESprite(desc) {

}

VOID Bullet::leaveSmoke() {
	AERO_SPRITE_DESC descSpr;
	descSpr.obj = ae_ObjectTable.getItem(3);
	descSpr.team = 0;
	descSpr.action = 0;
	descSpr.flip = SpriteEffects_None;
	descSpr.cx = cx;
	descSpr.cy = cy;
	descSpr.layerDepth = 0.5f;
	AESprite* spr_smoke = new AESprite(descSpr);
	XMFLOAT2 randomSpeed = AENSMath::randomPointWithinCircle(2.0f);
	spr_smoke->setVx(randomSpeed.x);
	spr_smoke->setVy(randomSpeed.y);
	scene->addSprite(spr_smoke);
}

VOID Bullet::update(AEHashedTable<AEPlatform>* platformTable) {
	vx = speed * cosf(angle);
	vy = speed * sinf(angle);
	AESprite::update(platformTable);
}


Shell::Shell(AERO_SPRITE_DESC desc) : AESprite(desc) {

}

VOID Shell::platformCollision(AEHashedTable<AEPlatform>* platformTable, AECollisionResult collisionResult) {
	AESprite::platformCollision(platformTable, collisionResult);
	kill();
}