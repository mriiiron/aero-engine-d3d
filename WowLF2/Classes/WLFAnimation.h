#pragma once


struct WLFAttackJudgeArea {

    RECT rect;
    INT effect, angle;
    INT spellID;

    WLFAttackJudgeArea() {
        rect = { 0, 0, 0, 0 };
        angle = 0;
        effect = -1;
        spellID = 0;
    }

};

struct WLFBodyJudgeArea {

    RECT rect;

    WLFBodyJudgeArea() {
        rect = { 0, 0, 0, 0 };
    }

};

struct WLFSpriteCreatePoint {

    static const INT FLIP_SAME_AS_HOST = 0;
    static const INT FLIP_ALWAYS_RIGHT = 1;
    static const INT FLIP_ALWAYS_LEFT = 2;
    static const INT FLIP_OPPOSITE_TO_HOST = 3;

    AERO_SPRITE_DESC descSprite;
    INT x, y;
    INT oid;
    INT count;
    INT flip;

    WLFSpriteCreatePoint() {
        x = y = 0;
        oid = 0;
        count = 1;
        flip = FLIP_SAME_AS_HOST;
    }

};

struct WLFCameraShakeOptions {

    INT time;
    INT amplitude;

    WLFCameraShakeOptions() {
        time = 0;
        amplitude = 3;
    }

};

struct WLFCancelOptions {

    INT toAction;
    INT moveInput;

    WLFCancelOptions() {
        toAction = 0;
        moveInput = 0;
    }

};


class WLFAnimation : public AEAnimation {

public:

    WLFAnimation(AERO_ANIMATION_DESC desc);
    ~WLFAnimation();

    INT getXShift(INT index) { return xShiftTable[index]; }
    INT getHitGroundAction() { return hitGroundAction; }
    WLFAttackJudgeArea* getAttackJudge(INT index) { return attackJudgeTable[index]; }
    WLFBodyJudgeArea* getBodyJudge(INT index) { return bodyJudgeTable[index]; }
    WLFSpriteCreatePoint* getSpriteCreate(INT index) { return spriteCreateTable[index]; }
    WLFCameraShakeOptions* getCameraShakeOptions(INT index) { return cameraShakeTable[index]; }
    WLFCancelOptions* getCancelOptions(INT index) { return cancelTable[index]; }
    XMFLOAT2 getGivenSpeed(INT index) { return giveSpeedTable[index]; }

    VOID addXShiftForFrame(INT index, INT xShift);
    VOID addAttackJudgeForFrame(INT index, WLFAttackJudgeArea* attackJudgeArea);
    VOID addBodyJudgeForFrame(INT index, WLFBodyJudgeArea* bodyJudgeArea);
    VOID addSpriteCreateForFrame(INT index, WLFSpriteCreatePoint* bodyJudgeArea);
    VOID addCameraShakeForFrame(INT index, WLFCameraShakeOptions* cameraShakeOptions);
    VOID addCancelForFrame(INT index, WLFCancelOptions* cancelOptions);
    VOID giveSpeedForFrame(INT index, XMFLOAT2 vec);
    VOID setTurnAfterAnim(BOOLEAN _isTurn) { isTurn = _isTurn; }
    VOID setHitGroundAction(INT _hitGroundAction) { hitGroundAction = _hitGroundAction; }

    BOOLEAN isTurnAfterAnim() { return isTurn; }

private:

    INT xShiftTable[MAX_FRAME_COUNT];
    WLFAttackJudgeArea* attackJudgeTable[MAX_FRAME_COUNT];
    WLFBodyJudgeArea*  bodyJudgeTable[MAX_FRAME_COUNT];
    WLFSpriteCreatePoint* spriteCreateTable[MAX_FRAME_COUNT];
    WLFCameraShakeOptions* cameraShakeTable[MAX_FRAME_COUNT];
    XMFLOAT2 giveSpeedTable[MAX_FRAME_COUNT];
    WLFCancelOptions* cancelTable[MAX_FRAME_COUNT];
    BOOLEAN isTurn;
    INT hitGroundAction;

};