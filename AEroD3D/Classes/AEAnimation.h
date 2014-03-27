#pragma once


#include "AEResource.h"


class AEFrameAttrib {

public:

	virtual ~AEFrameAttrib() {}

};


struct AERO_FRAME_DESC {

	AEResource* res;
	INT imgOffset, imgCells;
	INT centerx, centery;
	INT shiftx, shifty, dvx, dvy;

	AERO_FRAME_DESC() {
		res = nullptr;
		imgOffset = imgCells = 0;
		centerx = centery = 0;
		shiftx = shifty = dvx = dvy = 0;
	}

};


class AEFrame {

public:

	static const INT MAX_ATTRIB_COUNT			= 20;
	
	AEFrame(AERO_FRAME_DESC desc);
	~AEFrame();

	INT getImgOffset() { return imgOffset; }
	INT getImgCells() { return imgCells; }
	INT getWidth() { return width; }
	INT getHeight() { return height; }
	INT getCenterx() { return centerx; }
	INT getCentery() { return centery; }
	INT getShiftx() { return shiftx; }
	INT getShifty() { return shifty; }
	INT getDvx() { return dvx; }
	INT getDvy() { return dvy; }
	AEResource* getResource() { return res; }
	AEFrameAttrib* getOptional(INT index) { return attribTable[index]; }

	virtual VOID addOptionalByStrAt(std::string line);

private:

	AEResource* res;
	INT imgOffset, imgCells;
	INT width, height, centerx, centery;
	INT shiftx, shifty, dvx, dvy;
	AEFrameAttrib* attribTable[MAX_ATTRIB_COUNT];

};


struct AERO_ANIMATION_DESC {

	INT frameCount;
	INT timeToLive, next;
	BOOLEAN isAnimLoop;
	INT state;

	AERO_ANIMATION_DESC() {
		frameCount = 0;
		timeToLive = next = 0;
		isAnimLoop = FALSE;
		state = 0;
	}

};


// Animation Class: Arranges what and how frames should be displayed.
class AEAnimation {

public:

	static const INT MAX_FRAME_COUNT			= 100;

	AEAnimation(AERO_ANIMATION_DESC desc);
	~AEAnimation();
	
	AEFrame* getFrame(INT index) { return frameTable[index]; }
	INT getEndTime(INT index) { return endTimeTable[index]; }
	INT getFrameCount() { return frameCount; }
	BOOLEAN isLoop() { return isAnimLoop; }
	INT getNext() { return next; }
	INT getTTL() { return timeToLive; }
	INT getState() { return state; }

	VOID addFrame(INT index, AEFrame* frame, INT endTime);
	VOID cloneFrame(INT srcIndex, INT dstIndex);
	VOID loadFrameOptional(INT index, INT _slot, std::string str);

private:

	// Defining frames. Each frame has a time value to indicate that when it would change to next frame.
	INT frameCount;
	AEFrame* frameTable[MAX_FRAME_COUNT];
	INT endTimeTable[MAX_FRAME_COUNT];

	// Indicating whether this animation is loopable.
	// If so, then we should define its time to live. When it is expired, it would change to its next animation.
	// If not so, this animation would change to next animation just when it finishes.
	INT timeToLive, next;
	
	BOOLEAN isAnimLoop;

	// When displaying this Animation, the object is at which state.
	INT state;

};