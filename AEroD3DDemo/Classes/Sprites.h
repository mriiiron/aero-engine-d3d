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

	VOID platformCollision(AEPlatform* platform, INT tailNodeIndex, AECollisionResult collisionResult);
	VOID update(AEHashedTable<AEPlatform>* platformTable = nullptr);

	VOID shoot();

	BOOLEAN isFireKeyPressed, isRocketCoolingDown;

private:

	INT coolingDownTimer;
	INT rocketRemaining;

	FLOAT pitchAngleBalanced, pitchAngleMax, pitchAngleMin, pitchAngleRestoreSpeed;

};


class FlakCannon : public AESprite {

public:

	FlakCannon(AERO_SPRITE_DESC desc);

	VOID shoot();
	VOID throwShell();
	VOID emitSmoke();

private:

	

};


class Bullet : public AESprite {

public:

	Bullet(AERO_SPRITE_DESC desc);

	VOID platformCollision(AEPlatform* platform, INT tailNodeIndex, AECollisionResult collisionResult);
	VOID update(AEHashedTable<AEPlatform>* platformTable = nullptr);

	FLOAT getSpeed() { return speed; }
	VOID setSpeed(FLOAT _flipSpeed) { speed = _flipSpeed; }
	VOID leaveSmoke();

private:

	FLOAT speed = 0.0f;

};


class Shell : public AESprite {

public:

	Shell(AERO_SPRITE_DESC desc);

	VOID platformCollision(AEPlatform* platform, INT tailNodeIndex, AECollisionResult collisionResult);
	VOID update(AEHashedTable<AEPlatform>* platformTable = nullptr);

private:



};