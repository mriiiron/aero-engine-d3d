#pragma once


#include "AEResource.h"


class AEFrameAttrib {

public:

	virtual ~AEFrameAttrib() {}

};


class AEFrame {

public:

	static const INT MAX_ATTRIB_COUNT			= 20;
	
	VOID setImageFromRes(AEResource* _res, INT _imgOffset, INT _imgCells) { res = _res;  imgOffset = _imgOffset;  imgCells = _imgCells; }
	VOID setSize(INT _width, INT _height) { width = _width;  height = _height; }
	VOID setCenter(INT _centerx, INT _centery) { centerx = _centerx;  centery = _centery; }
	VOID setShift(INT _shiftx, INT _shifty) { shiftx = _shiftx;  shifty = _shifty; }
	VOID setDv(INT _dvx, INT _dvy) { dvx = _dvx;  dvy = _dvy; }
	AEResource* getResource() { return res; }
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

	AEFrameAttrib* getOptional(INT index) { return optionalParam[index]; }
	VOID addOptionalByStrAt(std::string line);

private:

	AEResource* res;
	INT imgOffset, imgCells;
	INT width, height, centerx, centery;
	INT shiftx, shifty, dvx, dvy;
	AEFrameOptional* optionalParam[MAX_ATTRIB_COUNT];

};


struct AERO_FRAME_DESC {

	AEResource* res;
	INT imgOffset, imgCells;
	INT width, height, centerx, centery;
	INT shiftx, shifty, dvx, dvy;

	AERO_FRAME_DESC() {
		res = nullptr;
		imgOffset = imgCells = 0;
		width = height = centerx = centery = 0;
		shiftx = shifty = dvx = dvy = 0;
	}

};

// Animation Class: Arranges what and how frames should be displayed.
class AEAnimation {

public:

	static const INT MAX_FRAMES				= 100;

	AEAnimation(INT _frameCount);
	
	VOID cloneFrame(INT srcIndex, INT dstIndex);
	VOID setLoop(INT _isLoop) { isAnimLoop = _isLoop; }
	VOID setState(INT _state) { state = _state; }
	VOID setNext(INT _next) { next = _next; }
	VOID setTTL(INT _timeToLive) { timeToLive = _timeToLive; }
	VOID setEndTime(INT index, INT _endTime) { endTime[index] = _endTime; }
	VOID setFrameCenter(INT index, INT _centerx, INT _centery) { frame[index]->setCenter(_centerx, _centery); }
	VOID setShift(INT index, INT _shiftx, INT _shifty) { frame[index]->setShift(_shiftx, _shifty); }
	VOID setDv(INT index, INT _dvx, INT _dvy) { frame[index]->setDv(_dvx, _dvy); }
	VOID setFrameImage(INT index, INT _rid, INT _offset, INT _cells);
	AEFrame* getFrame(INT index) { return frame[index]; }
	INT getEndTime(INT index) { return endTime[index]; }
	INT getFrameCount() { return frameCount; }
	BOOLEAN isLoop() { return isAnimLoop; }
	INT getNext() { return next; }
	INT getTTL() { return timeToLive; }
	INT getState() { return state; }

	virtual VOID loadFrameOptional(INT index, INT _slot, std::string str) {}

private:

	// Defining frames. Each frame has a time value to indicate that when it would change to next frame.
	INT frameCount;
	AEFrame* frame[MAX_FRAMES];
	INT* endTime;

	// Indicating whether this animation is loopable.
	// If so, then we should define its time to live. When it is expired, it would change to its next animation.
	// If not so, this animation would change to next animation just when it finishes.
	INT timeToLive, next;
	
	BOOLEAN isAnimLoop;

	// When displaying this Animation, the object is at which state.
	INT state;

};