#pragma once


struct WLFJudgeAreaRect {
	INT type;
	RECT rect;
};


class WLFAnimation : public AEAnimation {

public:

	static const int JUDGE_TYPE_ATTACK = 0;
	static const int JUDGE_TYPE_ON_HIT = 1;

	WLFAnimation(AERO_ANIMATION_DESC desc);

	INT getXShift(INT index) { return xShiftTable[index]; }
	WLFJudgeAreaRect* getJudgeArea(INT index) { return judgeTable[index]; }

	VOID addXShiftForFrame(INT index, INT xShift);
	VOID addJudgeAreaForFrame(INT index, WLFJudgeAreaRect* judgeArea);

private:

	INT xShiftTable[MAX_FRAME_COUNT];
	WLFJudgeAreaRect* judgeTable[MAX_FRAME_COUNT];

};