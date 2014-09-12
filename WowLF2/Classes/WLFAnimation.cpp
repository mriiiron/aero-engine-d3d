#include "AEroEngine.h"
#include "WLFAnimation.h"

WLFAnimation::WLFAnimation(AERO_ANIMATION_DESC desc) : AEAnimation(desc) {
	for (INT i = 0; i < MAX_FRAME_COUNT; i++) {
		xShiftTable[i] = 0;
		judgeTable[i] = nullptr;
	}
}

VOID WLFAnimation::addXShiftForFrame(INT index, INT xShift) {
	if (index < 0 || index >= frameCount) {
		// Error
		return;
	}
	xShiftTable[index] = xShift;
}

VOID WLFAnimation::addJudgeAreaForFrame(INT index, WLFJudgeAreaRect* judgeArea) {
	if (index < 0 || index >= frameCount) {
		// Error
		return;
	}
	judgeTable[index] = judgeArea;
}