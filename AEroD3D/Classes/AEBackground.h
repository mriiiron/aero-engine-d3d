/**
 * @file	AEBackground.h
 * @brief	Including the definitions all about game backgrounds.
 *
 * The backgrounds is included in Scenes. in AEro Engine, they are divided into several layers and several AELandforms.
 * Surficial Sprites could stand on these AELandforms. Of cource they also can jump up and down among those AELandforms.
 * The using of layers could implement animations, visual effects, etc. 
 *
 * @author	CaiyZ (Mr.IroN)
 * @version	0.1
 * @date	2013
*/

#pragma once


struct AELandformElem {
    INT altitude;
    FLOAT slope;
};


struct AELandform {
    AELandformElem* data;
    INT left, right;
};


struct AEBGLayerFrame {

    AEResource* res;
    INT imgOffset;

    AEBGLayerFrame(AEResource* _res, INT _imgOffset) {
        res = _res;
        imgOffset = _imgOffset;
    }

};


struct AERO_BGLAYERANIM_DESC {
    
    INT frameCount;

    AERO_BGLAYERANIM_DESC() {
        frameCount = 0;
    }

};

class AEBGLayerAnim {

public:

    static const INT MAX_FRAME_COUNT			= 100;

    AEBGLayerAnim(AERO_BGLAYERANIM_DESC desc);
    ~AEBGLayerAnim();

    AEBGLayerFrame* getFrame(INT index) { return frameTable[index]; }
    INT getEndTime(INT index) { return endTimeTable[index]; }
    INT getFrameCount() { return frameCount; }

    VOID addFrame(INT index, AEBGLayerFrame* frame, INT endTime);

private:

    AEBGLayerFrame* frameTable[MAX_FRAME_COUNT];
    INT endTimeTable[MAX_FRAME_COUNT];
    INT frameCount;

};


struct AEBGAnimRef {

    INT x, y;
    INT animIndex;
    INT time, frame;

    AEBGAnimRef(INT _x, INT _y, INT _animIndex) {
        x = _x;  y = _y;
        animIndex = _animIndex;
        time = frame = 0;
    }

};


struct AERO_BGLAYER_DESC {
    
    INT depth, width, height;
    FLOAT locX, locY;

    AERO_BGLAYER_DESC() {
        depth = width = height = 0;
        locX = locY = 0.0f;
    }

};


class AEBGLayer {

public:

    static const INT MAX_LAYER_ANIMS			= 20;

    AEBGLayer(AERO_BGLAYER_DESC desc);
    INT getDepth() { return depth; }
    INT getWidth() { return width; }
    INT getHeight() { return height; }
    INT getAnimCount() { return animCount; }
    INT getTimeOfAnim(INT index) { return animTable[index]->time; }
    INT getFrameOfAnim(INT index) { return animTable[index]->frame; }
    AEBGAnimRef* getAnimRef(INT index) { return animTable[index]; }
    AEPoint getLocation() { return AEPoint(locX, locY); }

    VOID addAnimRef(AEBGAnimRef* ref);
    VOID incTimeForAnim(INT index);
    VOID incFrameForAnim(INT index);
    VOID resetAnim(INT index);

private:

    INT depth, width, height;
    FLOAT locX, locY;
    INT animCount;
    AEBGAnimRef* animTable[MAX_LAYER_ANIMS];

};


struct AERO_BACKGROUND_DESC {

    std::string name;

    AERO_BACKGROUND_DESC() {
        name = "Unknown Background";
    }

};


class AEBackground {

public:

    static const INT MAX_LAYER_COUNT			= 10;
    static const INT MAX_ANIM_COUNT				= 50;
    static const INT MAX_LANDFORM_COUNT			= 50;
    static const INT ON_LANDFORM_TOLERANCE		= 2;

    AEBackground(AERO_BACKGROUND_DESC desc);

    INT getAELandformIndexBelow(INT cx, INT cy, INT* drop);
    INT getLayerCount() { return layerCount; }
    AELandform* getAELandform(INT index) { return landformTable[index]; }
    AEBGLayer* getLayer(INT index) { return layerTable[index]; }

    VOID addLayer(AEBGLayer* _layer);
    VOID loadAELandformsFromMonochrome(BYTE* pixels, INT width, INT height, INT byteLine);
    VOID loadAELandforms(BYTE* pixels, INT width, INT height);
    VOID addAnimAt(INT index, AEBGLayerAnim* layerAnim);

    VOID update();
    VOID render(XMFLOAT2 ae_CameraCenter);

private:

    std::string name;
    INT layerCount, landformCount;
    AEBGLayer* layerTable[MAX_LAYER_COUNT];
    AELandform* landformTable[MAX_LANDFORM_COUNT];
    AEBGLayerAnim* animTable[MAX_ANIM_COUNT];

};


class AEBackgroundLibrary {

public:

    static const INT MAX_BG_COUNT				= 30;

    AEBackgroundLibrary();

    AEBackground* get(INT index) { return lib[index]; }

    VOID add(AEBackground* bg);

private:

    AEBackground* lib[MAX_BG_COUNT];
    INT maxIndex;

};