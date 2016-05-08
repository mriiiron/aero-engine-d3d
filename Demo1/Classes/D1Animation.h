#pragma once


class D1Frame : public AEFrame {

public:

    D1Frame(AERO_FRAME_DESC desc);
    ~D1Frame();

    AERect* getAtkJudge() { return atkJudge; }
    AERect* getBdyJudge() { return bdyJudge; }
    VOID setAtkJudge(AERect* _atkJudge) { atkJudge = _atkJudge; }
    VOID setBdyJudge(AERect* _bdyJudge) { bdyJudge = _bdyJudge; }

protected:

    AERect* atkJudge;
    AERect* bdyJudge;

};


struct D1AnimOption_CreateSprite {

    AERO_SPRITE_DESC descSprite;
    INT x, y;
    INT oid;
    FLOAT vx, vy;

    D1AnimOption_CreateSprite() {
        x = y = 0;
        oid = 0;
        vx = vy = 0.0f;
        descSprite.obj = nullptr;
        descSprite.cx = 0.0f;
        descSprite.cy = 0.0f;
        descSprite.action = 0;
        descSprite.layerDepth = 0.0f;
        descSprite.scale = 1.0f;
        descSprite.team = 0;
        descSprite.flip = SpriteEffects::SpriteEffects_None;
    }

};


class D1Animation : public AEAnimation {

public:

    D1Animation(AERO_ANIMATION_DESC desc);
    ~D1Animation();

    VOID addSpriteCreationForFrame(INT index, D1AnimOption_CreateSprite* cs);
    D1AnimOption_CreateSprite* getSpriteCreationOption(INT index) { return spriteCreationTable[index]; }
    AERect* getBodyJudge(INT index) { return ((D1Frame*)(frameTable[index]))->getBdyJudge(); }
    AERect* getAttackJudge(INT index) { return ((D1Frame*)(frameTable[index]))->getAtkJudge(); }

private:

    D1AnimOption_CreateSprite* spriteCreationTable[MAX_FRAME_COUNT];

};