#pragma once


struct WLFAttackJudgeArea {

	RECT rect;
	INT effect, angle;

	WLFAttackJudgeArea() {
		rect = { 0, 0, 0, 0 };
		effect = angle = 0;
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


class WLFAnimation : public AEAnimation {

public:

	WLFAnimation(AERO_ANIMATION_DESC desc);
	~WLFAnimation();

	INT getXShift(INT index) { return xShiftTable[index]; }
	WLFAttackJudgeArea* getAttackJudge(INT index) { return attackJudgeTable[index]; }
	WLFBodyJudgeArea* getBodyJudge(INT index) { return bodyJudgeTable[index]; }
	WLFSpriteCreatePoint* getSpriteCreate(INT index) { return spriteCreateTable[index]; }

	VOID addXShiftForFrame(INT index, INT xShift);
	VOID addAttackJudgeForFrame(INT index, WLFAttackJudgeArea* attackJudgeArea);
	VOID addBodyJudgeForFrame(INT index, WLFBodyJudgeArea* bodyJudgeArea);
	VOID addSpriteCreateForFrame(INT index, WLFSpriteCreatePoint* bodyJudgeArea);

private:

	INT xShiftTable[MAX_FRAME_COUNT];
	WLFAttackJudgeArea* attackJudgeTable[MAX_FRAME_COUNT];
	WLFBodyJudgeArea*  bodyJudgeTable[MAX_FRAME_COUNT];
	WLFSpriteCreatePoint* spriteCreateTable[MAX_FRAME_COUNT];

};