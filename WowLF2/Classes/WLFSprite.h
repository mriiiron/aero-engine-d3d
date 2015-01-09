#pragma once


struct WLFCharacterHUDItemIndexes {
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
	static const INT ACTION_LAND_DEFAULT = 9;
	static const INT ACTION_HIT_FRONT_LOWER = 20;
	static const INT ACTION_HIT_FRONT_UPPER = 21;
	static const INT ACTION_HIT_BACK_LOWER = 22;
	static const INT ACTION_HIT_BACK_UPPER = 23;

	static const UINT KEY_LEFT = 0x1;
	static const UINT KEY_RIGHT = 0x2;
	static const UINT KEY_UP = 0x4;
	static const UINT KEY_DOWN = 0x8;
	static const UINT KEY_ATTACK_A = 0x10;
	static const UINT KEY_ATTACK_B = 0x20;
	static const UINT KEY_JUMP = 0x40;
	static const UINT KEY_SPECIAL = 0x80;
	static const UINT KEY_CHANGE_TARGET = 0x100;

	WLFCharacter(AERO_SPRITE_DESC desc);

	VOID platformCollision(AEPlatform* platform, INT tailNodeIndex, AECollisionResult collisionResult);
	VOID update(AEHashedTable<AEPlatform>* platformTable = nullptr);
	VOID render(INT renderOption = RENDER_OPTION_NORMAL, ...);

	VOID toBattleStance();
	VOID toStand();
	VOID attack_1();
	VOID attack_2();
	VOID changeTarget();

	VOID adsorbToPlatform();
	BOOLEAN isAttackLocked() { return attackLock; }
	BOOLEAN hasTarget() { if (target == nullptr) return FALSE; else return TRUE; }
	VOID setPortraitIndex(INT _portraitIndex) { portraitIndex = _portraitIndex; }
	INT getPortraitIndex() { return portraitIndex; }
	VOID setAttackLock(BOOLEAN lock) { attackLock = lock; }
	WLFCharacterHUDItemIndexes getHUDItemIndexes() { return hudItems; }
	VOID setHUDItemIndexes(INT namepad, INT portrait, INT bar_hp, INT bar_energy) { hudItems = { namepad, portrait, bar_hp, bar_energy }; }
	AEHashedTable<WLFBuff>* getBuffTable() { return buffTable; }

	VOID addBuff(WLFBuff* buff, WLFCharacter* caster);
	VOID updateBuffTable();

	VOID shake(INT time, INT amplitude);

protected:

	AEHashedTable<WLFBuff>* buffTable;
	WLFCharacterHUDItemIndexes hudItems;

	INT portraitIndex;
	INT targetIndexHash;
	WLFCharacter* target;
	AEPlatform* onPlatform;
	INT onPlatformTailIndex;
	BOOLEAN attackLock;
	INT shakeTimer, shakeAmp;

};


class WLFWarrior : public WLFCharacter {

public:

	WLFWarrior(AERO_SPRITE_DESC desc);

	VOID update(AEHashedTable<AEPlatform>* platformTable = nullptr);

	INT getRage() { return rage; }
	INT getRageMax() { return rageMax; }
	FLOAT getRageProportion() { return 1.0f * rage / rageMax; }

	VOID mortalStrike();
	VOID overpower();
	VOID slam();
	VOID colossusSmash();
	VOID thunderClap();
	VOID charge();

private:

	FLOAT chargeTargetPosX;
	INT rage, rageMax;

};


class WLFBar : public AESprite {

public:

	static const INT TYPE_HP = 0;
	static const INT TYPE_ENERGY = 1;

	WLFBar(AERO_SPRITE_DESC desc, WLFCharacter* _host, INT _type);

	VOID update(AEHashedTable<AEPlatform>* platformTable = nullptr);
	VOID render(INT renderOption = RENDER_OPTION_NORMAL, ...);

private:

	WLFCharacter* host;
	INT type;

};


class WLFBuffIcon : public AESprite {

public:

	WLFBuffIcon(AERO_SPRITE_DESC desc, WLFBuff* _buff);

	VOID render(INT renderOption = RENDER_OPTION_NORMAL, ...);

private:

	WLFBuff* buff;

};