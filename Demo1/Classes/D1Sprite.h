#pragma once


class D1Key {

public:

    static const UINT KEY_LEFT      = 0x1;
    static const UINT KEY_RIGHT     = 0x2;
    static const UINT KEY_UP        = 0x4;
    static const UINT KEY_DOWN      = 0x8;
    static const UINT KEY_ATTACK    = 0x10;
    static const UINT KEY_BOMB      = 0x20;

};


class D1Sprite : public AESprite {

public:

    D1Sprite(AERO_SPRITE_DESC desc);

protected:

    VOID update(AEHashedTable<AEPlatform>* platformTable = nullptr);
    VOID render(INT renderOption = RENDER_OPTION_NORMAL, ...);

};


class D1PlayerFighter : public D1Sprite {

public:

    static const INT ACTION_IDLE                = 0;
    static const INT ACTION_MOVE_LEFT           = 1;
    static const INT ACTION_MOVE_LEFT_HOLD      = 2;
    static const INT ACTION_MOVE_LEFT_RECOVER   = 3;
    static const INT ACTION_MOVE_RIGHT          = 4;
    static const INT ACTION_MOVE_RIGHT_HOLD     = 5;
    static const INT ACTION_MOVE_RIGHT_RECOVER  = 6;
    static const INT ACTION_KILLED              = 10;

    D1PlayerFighter(AERO_SPRITE_DESC desc);

    VOID update(AEHashedTable<AEPlatform>* platformTable = nullptr);
    VOID render(INT renderOption = RENDER_OPTION_NORMAL, ...);

    VOID shoot(FLOAT xOffset = 0.0f);
    VOID attack();

protected:

    INT attackTimer;

};


class D1EnemyFighter : public D1Sprite {

public:

    static const INT ACTION_IDLE            = 0;
    static const INT ACTION_SPIN            = 1;
    static const INT ACTION_SPIN_RECOVER    = 2;
    static const INT ACTION_KILLED          = 10;

    D1EnemyFighter(AERO_SPRITE_DESC desc);

    VOID update(AEHashedTable<AEPlatform>* platformTable = nullptr);
    VOID render(INT renderOption = RENDER_OPTION_NORMAL, ...);

    VOID rocketAttack(FLOAT xOffset = 0.0f, FLOAT yOffset = 0.0f);
    VOID bulletAttack(FLOAT vx, FLOAT vy, FLOAT xOffset = 0.0f, FLOAT yOffset = 0.0f);
    VOID vulcanAttack();

protected:

};


