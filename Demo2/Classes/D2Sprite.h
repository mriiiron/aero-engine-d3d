#pragma once


class D2Sprite : public AESprite {

public:

    static const INT STATE_IN_AIR = 0;
    static const INT STATE_ON_GROUND = 1;

    static const INT ACTION_IDLE = 0;
    static const INT ACTION_RUN = 1;
    static const INT ACTION_JUMP = 2;

    D2Sprite(AERO_SPRITE_DESC desc);

    VOID platformCollision(AEPlatform* platform, INT tailNodeIndex, AECollisionResult collisionResult);
    VOID update(AEHashedTable<AEPlatform>* platformTable = nullptr);
    VOID render(INT renderOption = RENDER_OPTION_NORMAL, ...);

protected:

    VOID absorbToPlatform();

    AEPlatform* onPlatform;
    INT onPlatformTailIndex;
    INT moving;

};