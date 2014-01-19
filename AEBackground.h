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


typedef struct AELandformElem {
	INT altitude;
	FLOAT slope;
};


typedef struct AELandform {
	AELandformElem* data;
	INT left, right;
};


typedef struct AEBGLayerFrame {
	INT width, height;
	INT rid;
	INT imgOffset;
};


class AEBGLayerAnim {

public:

	AEBGLayerAnim() { frames = NULL;  endTimes = NULL;  frameCount = 0; }
	VOID setFrameImage(INT index, INT _rid, INT _offset, INT _width, INT _height);
	VOID setFrameCount(INT _frameCount) { frameCount = _frameCount;  frames = new AEBGLayerFrame[_frameCount];  endTimes = new INT[_frameCount];}
	VOID setEndTime(INT index, INT _endTime) { endTimes[index] = _endTime; }
	AEBGLayerFrame getFrame(INT index) { return frames[index]; }
	INT getEndTime(INT index) { return endTimes[index]; }
	INT getFrameCount() { return frameCount; }

private:

	AEBGLayerFrame* frames;
	INT* endTimes;
	INT frameCount;

};


typedef struct AEAnimRef {
	INT x, y;
	INT animIndex;
	INT time, frame;
};


class AEBGLayer {

public:

	static const INT MAX_LAYER_ANIMS			= 20;

	AEBGLayer(INT _rid, INT _depth, INT _width, INT _height, FLOAT _offsetx, FLOAT _offsety);
	INT getLayerRid() { return rid; }
	INT getDepth() { return depth; }
	INT getWidth() { return width; }
	INT getHeight() { return height; }
	INT getAnimCount() { return animCount; }
	INT getTimeOfAnim(INT index) { return anim[index].time; }
	INT getFrameOfAnim(INT index) { return anim[index].frame; }
	AEAnimRef getAEAnimRef(INT index) { return anim[index]; }
	AEPoint getOffsetPosition() { return offsetPosition; }
	VOID addAEAnimRef(AEAnimRef AEAnimRef) { anim[animCount] = AEAnimRef;  animCount++; }
	VOID incTimeForAnim(INT index) { anim[index].time = anim[index].time + 1; }
	VOID incFrameForAnim(INT index) { anim[index].frame = anim[index].frame + 1; }
	VOID resetAnim(INT index) { anim[index].time = anim[index].frame = 0; }

private:

	INT rid, depth, width, height;
	AEPoint offsetPosition;
	INT animCount;
	AEAnimRef anim[MAX_LAYER_ANIMS];

};


class AEBackground {

public:

	static const INT MAX_LAYER_COUNT			= 10;
	static const INT MAX_ANIM_COUNT				= 50;
	static const INT MAX_LANDFORM_COUNT			= 50;
	static const INT ONAELandform_TOLERANCE		= 2;

	AEBackground(AERO_BACKGROUND_DESC desc, AEBGLayer** _layerTable, AELandform** _landformTable, AEBGLayerAnim** _animTable);
	INT getXonBG(INT cx) { return cx - INT(location.x); }
	INT getYonBG(INT cy) { return cy - INT(location.y); }
	INT getAELandformIndexBelow(INT cx, INT cy, INT* drop);
	AELandform* getAELandform(INT index) { return landformTable[index]; }
	VOID loadAELandformsFromMonochrome(GLubyte* pixels, INT width, INT height, INT byteLine);
	VOID loadAELandforms(GLubyte* pixels, INT width, INT height);
	VOID addAnimAt(INT index, AEBGLayerAnim layerAnim);
	VOID update();
	VOID paint(AEPoint cameraCenter);
	AEPoint getLocation() { return location; }
	INT getWidth() { return width; }
	INT getHeight() { return height; }
	INT getLayerCount() { return layerCount; }
	AEBGLayer* getLayer(INT index) { return layerTable[index]; }
	VOID setLayer(INT index, AEBGLayer* _layer) { layerTable[index] = _layer; }

private:

	std::string name;
	AEPoint location;
	INT width, height;
	INT layerCount, landformCount;
	AEBGLayer* layerTable[MAX_LAYER_COUNT];
	AELandform* landformTable[MAX_LANDFORM_COUNT];
	AEBGLayerAnim* animTable[MAX_ANIM_COUNT];

};


struct AERO_BACKGROUND_DESC {

	std::string name;
	FLOAT locX, locY;
	INT width, height;
	INT layerCount, landformCount;

	AERO_BACKGROUND_DESC() {
		name = "Unknown Background";
		locX = locY = 0.0f;
		width = height = 0;
		layerCount = landformCount = 0;
	}

};


class AEBackgroundLibrary {

public:

	static const INT MAX_BG_COUNT				= 30;

	AEBackgroundLibrary();
	VOID add(AEBackground* bg);
	AEBackground* get(INT index) { return lib[index]; }

private:

	AEBackground* lib[MAX_BG_COUNT];
	AEConstantTable<AEResource, 100>* bgResTable;
	INT maxIndex;

};