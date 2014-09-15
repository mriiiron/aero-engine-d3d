#pragma once


class WLFCharacter : public AESprite {

public:

	static const INT STATE_IN_AIR = 0;
	static const INT STATE_ON_GROUND = 1;

	WLFCharacter(AERO_SPRITE_DESC desc);

	VOID platformCollision(AEPlatform* platform, INT tailNodeIndex, AECollisionResult collisionResult);
	VOID update(AEHashedTable<AEPlatform>* platformTable = nullptr);

	VOID toBattleStance();
	VOID toStand();
	VOID attack_1();
	VOID attack_2();

	VOID adsorbToPlatform();
	BOOLEAN isAttackLocked() { return attackLock; }
	VOID setAttackLock(BOOLEAN lock) { attackLock = lock; }

	BOOLEAN isRightKeyPressed, isLeftKeyPressed;

private:

	AEPlatform* onPlatform;
	INT onPlatformTailIndex;
	BOOLEAN attackLock;

};


class WLFDummy : public AESprite {

public:

	WLFDummy(AERO_SPRITE_DESC desc);

	VOID platformCollision(AEPlatform* platform, INT tailNodeIndex, AECollisionResult collisionResult);
	VOID update(AEHashedTable<AEPlatform>* platformTable = nullptr);

private:



};