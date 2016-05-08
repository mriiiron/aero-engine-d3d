#pragma once


class TemplateSprite : public AESprite {

public:

    TemplateSprite(AERO_SPRITE_DESC desc);

    VOID platformCollision(AEPlatform* platform, INT tailNodeIndex, AECollisionResult collisionResult);
    VOID update(AEHashedTable<AEPlatform>* platformTable = nullptr);
    VOID render(INT renderOption = RENDER_OPTION_NORMAL, ...);

protected:



};