#pragma once


#include <string>
#include "AEResource.h"


struct AERO_FRAME_DESC {

    AEResource* res;
    INT imgOffset, imgCells;
    INT centerx, centery;

    AERO_FRAME_DESC() {
        res = nullptr;
        imgOffset = 0;
        imgCells = 1;
        centerx = centery = 0;
    }

};


class AEFrame {

public:
    
    static const int MAX_ATTACH_SLOTS           = 100;

    AEFrame(AERO_FRAME_DESC desc);
    ~AEFrame();

    INT getImgOffset() { return imgOffset; }
    INT getImgCells() { return imgCells; }
    INT getWidth() { return width; }
    INT getHeight() { return height; }
    AEPointI getCenter() { return center; }
    AEResource* getResource() { return res; }
    AEPointI getAttachPoint(INT slot) { return attachSlotTable[slot]; }
    VOID addAttachSlot(INT slotNum, INT x, INT y) { attachSlotTable[slotNum] = AEPointI(x, y); }

protected:

    AEResource* res;
    INT imgOffset, imgCells;
    INT width, height;
    AEPointI center;
    AEPointI attachSlotTable[MAX_ATTACH_SLOTS];

};


struct AERO_ANIMATION_DESC {

    std::string name;
    INT frameCount;
    INT timeToLive, next;
    BOOLEAN isAnimLoop;

    AERO_ANIMATION_DESC() {
        name = "Unknown Animation";
        frameCount = 0;
        timeToLive = -1;
        next = 0;
        isAnimLoop = FALSE;
    }

};


// Animation Class: Arranges what and how frames should be displayed.
class AEAnimation {

public:

    static const INT MAX_FRAME_COUNT			= 200;

    AEAnimation(AERO_ANIMATION_DESC desc);
    ~AEAnimation();
    
    std::string getName() { return name; }
    AEFrame* getFrame(INT index) { return frameTable[index]; }
    INT getEndTime(INT index) { return endTimeTable[index]; }
    INT getFrameCount() { return frameCount; }
    BOOLEAN isLoop() { return isAnimLoop; }
    INT getNext() { return next; }
    INT getTTL() { return timeToLive; }
    INT getState() { return state; }

    VOID setTTL(INT _timeToLive) { timeToLive = _timeToLive; }
    VOID addFrame(INT index, AEFrame* frame, INT endTime);

protected:

    std::string name;

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