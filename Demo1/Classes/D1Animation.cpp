#include "AEroEngine.h"

#include "D1Animation.h"


D1Frame::D1Frame(AERO_FRAME_DESC desc) : AEFrame(desc) {
    atkJudge = nullptr;
    bdyJudge = nullptr;
}

D1Frame::~D1Frame() {
    delete atkJudge;
    delete bdyJudge;
}


D1Animation::D1Animation(AERO_ANIMATION_DESC desc) : AEAnimation(desc) {
    for (INT i = 0; i < MAX_FRAME_COUNT; i++) {
        spriteCreationTable[i] = nullptr;
    }
}

D1Animation::~D1Animation() {
    delete[] spriteCreationTable;
}

VOID D1Animation::addSpriteCreationForFrame(INT index, D1AnimOption_CreateSprite* cs) {
    if (index < 0 || index >= frameCount) {
        AENSGameControl::exitGame("Error in Animation " + name + ":\nFrame number out of range.");
        return;
    }
    spriteCreationTable[index] = cs;
}
