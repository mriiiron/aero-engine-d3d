#pragma once


class JFighterSprite : public AESprite {

public:

	JFighterSprite(AERO_SPRITE_DESC desc);

	VOID update(AEHashedTable<AEPlatform>* platformTable = nullptr);

	VOID shoot();

	BOOLEAN isFireKeyPressed, isRocketCoolingDown;

private:

	INT currentRocketSlotIndex;
	INT rocketSlotOffset[6];
	INT coolingDownTimer;

};


class Turret : public AESprite {

public:

	Turret(AERO_SPRITE_DESC desc);

	VOID shootRocket();
	VOID shootHomingBullets();

private:

	INT firingSlot;

};


class Helicopter : public AESprite {

public:

	Helicopter(AERO_SPRITE_DESC desc);

	VOID platformCollision(AEHashedTable<AEPlatform>* platformTable, AECollisionResult collisionResult);
	VOID update(AEHashedTable<AEPlatform>* platformTable = nullptr);

	VOID shoot();

	BOOLEAN isFireKeyPressed, isRocketCoolingDown;

private:

	INT coolingDownTimer;

};


class Bullet : public AESprite {

public:

	Bullet(AERO_SPRITE_DESC desc);

	VOID update(AEHashedTable<AEPlatform>* platformTable = nullptr);

	FLOAT getFacingSpeed() { return facingSpeed; }
	VOID setFacingSpeed(FLOAT _facingSpeed) { facingSpeed = _facingSpeed; }

private:

	FLOAT facingSpeed = 0.0f;

};