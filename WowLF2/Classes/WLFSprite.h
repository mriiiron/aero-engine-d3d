#pragma once

#include <string>


struct WLFCharacterHUDItemIndexes {
    INT namepad, portrait, bar_hp, bar_energy;
};


class WLFCharacter : public AESprite {

public:

    static const INT STATE_IN_AIR = 0;
    static const INT STATE_ON_GROUND = 1;

    static const INT ACTION_STAND = 0;
    static const INT ACTION_RUN = 1;
    static const INT ACTION_RUN_END = 2;
    static const INT ACTION_JUMP = 3;
    static const INT ACTION_IN_AIR = 8;
    static const INT ACTION_LAND_DEFAULT = 9;
    static const INT ACTION_HIT_FRONT_LOWER = 10;
    static const INT ACTION_HIT_FRONT_UPPER = 11;
    static const INT ACTION_HIT_BACK_LOWER = 12;
    static const INT ACTION_HIT_BACK_UPPER = 13;
    static const INT ACTION_HIT_FRONT_HEAVY = 14;
    static const INT ACTION_HIT_BACK_HEAVY = 15;
    static const INT ACTION_HIT_FRONT_FALL = 16;
    static const INT ACTION_HIT_BACK_FALL = 17;

    static const INT MAX_MOVES_COUNT = 20;
    static const INT MOVE_A = 0;
    static const INT MOVE_FORWARD_A = 1;
    static const INT MOVE_DOWN_A = 2;
    static const INT MOVE_UP_A = 3;
    static const INT MOVE_B = 4;
    static const INT MOVE_FORWARD_B = 5;
    static const INT MOVE_DOWN_B = 6;
    static const INT MOVE_UP_B = 7;
    static const INT MOVE_JUMP = 8;

    WLFCharacter(AERO_SPRITE_DESC desc);

    VOID platformCollision(AEPlatform* platform, INT tailNodeIndex, AECollisionResult collisionResult);
    VOID update(AEHashedTable<AEPlatform>* platformTable = nullptr);
    VOID render(INT renderOption = RENDER_OPTION_NORMAL, ...);

    VOID changeTarget();

    VOID adsorbToPlatform();
    BOOLEAN isAttackLocked() { return attackLock; }
 
    VOID setPortraitIndex(INT _portraitIndex) { portraitIndex = _portraitIndex; }
    INT getPortraitIndex() { return portraitIndex; }
    VOID setAttackLock(BOOLEAN lock) { attackLock = lock; }
    WLFCharacterHUDItemIndexes getHUDItemIndexes() { return hudItems; }
    VOID setHUDItemIndexes(INT namepad, INT portrait, INT bar_hp, INT bar_energy) { hudItems = { namepad, portrait, bar_hp, bar_energy }; }
    AEHashedTable<WLFBuff>* getBuffTable() { return buffTable; }
    WLFCharacter* getTarget() { return target; }
    VOID setTarget(WLFCharacter* _target);
    VOID setPlayerFlag(BOOLEAN flag) { isPlayerFlag = flag; }
    BOOLEAN isPlayer() { return isPlayerFlag; }
 
    VOID applyMoveInput(INT move, INT tolerance = 5) { moveInputs[move] = tolerance; }
    VOID addBuff(WLFBuff* buff, WLFCharacter* caster);
    BOOLEAN hasBuff(std::string buffName);
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
    FLOAT fraction;
    BOOLEAN isFractionDisabled;
    INT moveInputs[MAX_MOVES_COUNT];
    BOOLEAN isPlayerFlag;

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
    VOID overpower_2();
    VOID slam();
    VOID colossusSmash();
    VOID thunderClap();
    VOID charge();

    VOID jump();

private:

    FLOAT chargeTargetPosX;
    INT rage, rageMax;

};


class WLFBandit : public WLFCharacter {

public:

    WLFBandit(AERO_SPRITE_DESC desc);

    VOID update(AEHashedTable<AEPlatform>* platformTable = nullptr);

    VOID frontKick();
    VOID maceAttack();

private:



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


class WLFFloatingText : public AESprite {

public:

    WLFFloatingText(AERO_SPRITE_DESC desc, std::string _text);

    VOID update(AEHashedTable<AEPlatform>* platformTable);
    VOID render(INT renderOption = RENDER_OPTION_NORMAL, ...);

private:

    std::string text;

};