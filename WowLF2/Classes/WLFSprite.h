#pragma once


class WLFCharacter : public AESprite {

public:

	static const INT STATE_IN_AIR = 0;
	static const INT STATE_ON_GROUND = 1;

	static const INT ACTION_STAND = 0;
	static const INT ACTION_BATTLE_STANCE = 1;
	static const INT ACTION_WALK = 2;
	static const INT ACTION_TURN = 3;
	static const INT ACTION_BATTLE_STANCE_START = 4;
	static const INT ACTION_BATTLE_STANCE_END = 5;
	static const INT ACTION_IN_AIR = 8;
	static const INT ACTION_LAND = 9;
	static const INT ACTION_HIT_FRONT_LOWER = 20;
	static const INT ACTION_HIT_FRONT_UPPER = 21;
	static const INT ACTION_HIT_BACK_LOWER = 22;
	static const INT ACTION_HIT_BACK_UPPER = 23;

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