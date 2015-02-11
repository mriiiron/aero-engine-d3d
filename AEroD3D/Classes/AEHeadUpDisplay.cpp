#include <d3d11_1.h>
#include <string>
#include "AEResource.h"
#include "AEHeadUpDisplay.h"

AEHeadUpDisplay::AEHeadUpDisplay(INT spriteTableSize) {
    scene = nullptr;
    spriteTable = new AEHashedTable<AESprite>(spriteTableSize);
}

VOID AEHeadUpDisplay::update() {
    if (spriteTable) {
        for (INT iHash = 0; iHash < spriteTable->getHashCount(); iHash++) {
            AESprite* sprite = spriteTable->getItemByHash(iHash);
            if (sprite->isDead()) {
                spriteTable->removeItemByHash(iHash);
            }
            else {
                sprite->update();
            }
        }
    }
}

VOID AEHeadUpDisplay::render() {
    if (spriteTable) {
        for (INT iHash = 0; iHash < spriteTable->getHashCount(); iHash++) {
            AESprite* sprite = spriteTable->getItemByHash(iHash);
            if (!(sprite->isDead())) {
                sprite->render();
            }
        }
    }
}