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


class WLFAnimation : public AEAnimation {

public:

	WLFAnimation(AERO_ANIMATION_DESC desc);
	~WLFAnimation();

	INT getXShift(INT index) { return xShiftTable[index]; }
	WLFAttackJudgeArea* getAttackJudge(INT index) { return attackJudgeTable[index]; }
	WLFBodyJudgeArea* getBodyJudge(INT index) { return bodyJudgeTable[index]; }

	VOID addXShiftForFrame(INT index, INT xShift);
	VOID addAttackJudgeForFrame(INT index, WLFAttackJudgeArea* attackJudgeArea);
	VOID addBodyJudgeForFrame(INT index, WLFBodyJudgeArea* bodyJudgeArea);

private:

	INT xShiftTable[MAX_FRAME_COUNT];
	WLFAttackJudgeArea* attackJudgeTable[MAX_FRAME_COUNT];
	WLFBodyJudgeArea*  bodyJudgeTable[MAX_FRAME_COUNT];

};