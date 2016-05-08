#include "AEroEngine.h"
#include "AI.h"
#include "Sprite.h"

extern AEConstantTable<AEObject>			ae_ObjectTable;


TemplateSprite::TemplateSprite(AERO_SPRITE_DESC desc) : AESprite(desc) {
    
}

VOID TemplateSprite::platformCollision(AEPlatform * platform, INT tailNodeIndex, AECollisionResult collisionResult) {
    AESprite::platformCollision(platform, tailNodeIndex, collisionResult);
}

VOID TemplateSprite::update(AEHashedTable<AEPlatform>* platformTable) {
    AESprite::update(platformTable);
}

VOID TemplateSprite::render(INT renderOption, ...) {
    AESprite::render(renderOption);
}
