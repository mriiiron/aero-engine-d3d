#pragma once


struct WLFCharacterHUDItemHashes {
	INT namepad, portrait, bar_hp, bar_energy;
};


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

	static const UINT KEY_LEFT = 0x1;
	static const UINT KEY_RIGHT = 0x2;
	static const UINT KEY_ATTACK_1 = 0x4;
	static const UINT KEY_ATTACK_2 = 0x8;
	static const UINT KEY_CHANGE_TARGET = 0x100;

	WLFCharacter(AERO_SPRITE_DESC desc);

	VOID platformCollision(AEPlatform* platform, INT tailNodeIndex, AECollisionResult collisionResult);
	VOID update(AEHashedTable<AEPlatform>* platformTable = nullptr);

	VOID toBattleStance();
	VOID toStand();
	VOID attack_1();
	VOID attack_2();
	VOID changeTarget();

	VOID adsorbToPlatform();
	BOOLEAN isAttackLocked() { return attackLock; }
	VOID setAttackLock(BOOLEAN lock) { attackLock = lock; }
	VOID setHUDItemHashes(INT namepad, INT portrait, INT bar_hp, INT bar_energy) { hudItems = { namepad, portrait, bar_hp, bar_energy }; }

private:

	INT targetIndexHash;
	WLFCharacter* target;
	AEPlatform* onPlatform;
	INT onPlatformTailIndex;
	BOOLEAN attackLock;

	WLFCharacterHUDItemHashes hudItems;

};