#include <d3d11_1.h>
#include <string>
#include "AEAnimation.h"


AEFrame::AEFrame(AERO_FRAME_DESC desc) {
	res = desc.res;
	imgOffset = desc.imgOffset;
	imgCells = desc.imgCells;
	width = desc.res->getCellWidth() * imgCells;
	height = desc.res->getCellHeight();
	centerx = desc.centerx;
	centery = desc.centery;
}

AEFrame::~AEFrame() {
	// TODO
}


AEAnimation::AEAnimation(AERO_ANIMATION_DESC desc) {
	name = desc.name;
	frameCount = desc.frameCount;
	isAnimLoop = desc.isAnimLoop;
	next = desc.next;
	timeToLive = desc.timeToLive;
	for (INT i = 0; i < MAX_FRAME_COUNT; i++) {
		frameTable[i] = nullptr;
		endTimeTable[i] = 0;
	}
}

AEAnimation::~AEAnimation() {
	for (INT i = 0; i < MAX_FRAME_COUNT; i++) {
		if (frameTable[i]) {
			delete frameTable[i];
			frameTable[i] = nullptr;
		}
	}
}

VOID AEAnimation::addFrame(INT index, AEFrame* frame, INT endTime) {
	if (index < 0 || index >= frameCount) {
		// Error
		return;
	}
	frameTable[index] = frame;
	endTimeTable[index] = endTime;
}