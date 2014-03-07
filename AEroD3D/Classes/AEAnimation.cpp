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
	shiftx = desc.shiftx;
	shifty = desc.shifty;
	dvx = desc.dvx;
	dvy = desc.dvy;
	for (INT i = 0; i < MAX_ATTRIB_COUNT; i++) {
		attribTable[i] = nullptr;
	}
}

VOID AEFrame::addOptionalByStrAt(std::string line) {

}


AEAnimation::AEAnimation(AERO_ANIMATION_DESC desc) {
	frameCount = desc.frameCount;
	isAnimLoop = desc.isAnimLoop;
	next = desc.next;
	state = desc.state;
	timeToLive = desc.timeToLive;
	for (INT i = 0; i < MAX_FRAME_COUNT; i++) {
		frameTable[i] = nullptr;
		endTimeTable[i] = 0;
	}
}

VOID AEAnimation::addEndTime(INT index, INT endTime) {
	if (index < 0 || index >= frameCount) {
		// Error
		return;
	}
	endTimeTable[index] = endTime;
}

VOID AEAnimation::addFrame(INT index, AEFrame* frame) {
	if (index < 0 || index >= frameCount) {
		// Error
		return;
	}
	frameTable[index] = frame;
}

VOID AEAnimation::cloneFrame(INT srcIndex, INT dstIndex) {

}

VOID AEAnimation::loadFrameOptional(INT index, INT _slot, std::string str) {

}

