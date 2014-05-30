#include <d3d11_1.h>
#include <string>
#include "AEMath.h"
#include "AEResource.h"
#include "AETable.h"
#include "AEBackground.h"

AEBGLayerAnim::AEBGLayerAnim(AERO_BGLAYERANIM_DESC desc) {
	frameCount = desc.frameCount;
	for (INT i = 0; i < MAX_FRAME_COUNT; i++) {
		frameTable[i] = nullptr;
		endTimeTable[i] = 0;
	}
}

AEBGLayerAnim::~AEBGLayerAnim() {
	// TODO
}

VOID AEBGLayerAnim::addFrame(INT index, AEBGLayerFrame* frame, INT endTime) {
	if (index < 0 || index >= frameCount) {
		// Error
		return;
	}
	frameTable[index] = frame;
	endTimeTable[index] = endTime;
}


AEBGLayer::AEBGLayer(AERO_BGLAYER_DESC desc) {
	depth = desc.depth;
	width = desc.width;
	height = desc.height;
	locX = desc.locX;  locY = desc.locY;
	animCount = 0;
	for (INT i = 0; i < MAX_LAYER_ANIMS; i++) {
		animTable[i] = nullptr;
	}
}

VOID AEBGLayer::addAnimRef(AEBGAnimRef* ref) {
	animTable[animCount] = ref;  animCount++;
}

VOID AEBGLayer::incTimeForAnim(INT index) {
	animTable[index]->time = animTable[index]->time + 1;
}

VOID AEBGLayer::incFrameForAnim(INT index) {
	animTable[index]->frame = animTable[index]->frame + 1;
}

VOID AEBGLayer::resetAnim(INT index) {
	animTable[index]->time = animTable[index]->frame = 0;
}



AEBackground::AEBackground(AERO_BACKGROUND_DESC desc) {
	name = desc.name;
	layerCount = landformCount = 0;
	for (int i = 0; i < MAX_LAYER_COUNT; i++) {
		layerTable[i] = nullptr;
	}
	for (int i = 0; i < MAX_LANDFORM_COUNT; i++) {
		landformTable[i] = nullptr;
	}
	for (int i = 0; i < MAX_ANIM_COUNT; i++) {
		animTable[i] = nullptr;
	}
}

VOID AEBackground::addLayer(AEBGLayer* _layer) {
	layerTable[layerCount] = _layer;
	layerCount++;
}

VOID AEBackground::loadAELandformsFromMonochrome(BYTE* pixels, INT width, INT height, INT byteLine) {

}

VOID AEBackground::loadAELandforms(BYTE* pixels, INT width, INT height) {

}

VOID AEBackground::addAnimAt(INT index, AEBGLayerAnim* layerAnim) {
	animTable[index] = layerAnim;
}

VOID AEBackground::update() {
	for (INT i = 0; i < layerCount; i++) {
		for (INT j = 0; j < layerTable[i]->getAnimCount(); j++) {
			layerTable[i]->incTimeForAnim(j);
			AEBGLayerAnim* anim = animTable[layerTable[i]->getAnimRef(j)->animIndex];
			if (layerTable[i]->getTimeOfAnim(j) >= anim->getEndTime(layerTable[i]->getFrameOfAnim(j))) {
				layerTable[i]->incFrameForAnim(j);
				if (layerTable[i]->getFrameOfAnim(j) == anim->getFrameCount()) {
					layerTable[i]->resetAnim(j);
				}
			}
		}
	}
}

VOID AEBackground::addToRenderBuffer(AEPoint ae_CameraCenter) {
	for (INT i = layerCount - 1; i >= 0; i--) {
		FLOAT dx = ae_CameraCenter.x;
		FLOAT depth = FLOAT(layerTable[i]->getDepth());
		FLOAT correction = dx * depth / 100.0f;
		for (INT j = 0; j < layerTable[i]->getAnimCount(); j++) {
			AEBGAnimRef* pRef = layerTable[i]->getAnimRef(j);
			AEBGLayerAnim* pLAnim = animTable[pRef->animIndex];
			AEBGLayerFrame* lf = pLAnim->getFrame(pRef->frame);
			AEResource* pRes = lf->res;
			INT width = pRes->getCellWidth();
			INT height = pRes->getCellHeight();
			FLOAT x1, x2, y1, y2;
			AEPoint layerPos = layerTable[i]->getLocation();
			x1 = layerPos.x + pRef->x + correction;
			x2 = x1 + width;
			y1 = layerPos.y + pRef->y;
			y2 = y1 + height;
			AERect animRect(x1, y1, x2, y2);
			AERect texClip = lf->res->getTexClip(lf->imgOffset, 1);
			lf->res->addToRenderBuffer(animRect, texClip, 1.0f, 100.0f);
		}
	}
}


AEBackgroundLibrary::AEBackgroundLibrary() {
	maxIndex = 0;
	for (INT i = 0; i < MAX_BG_COUNT; i++) {
		lib[i] = NULL;
	}
}

VOID AEBackgroundLibrary::add(AEBackground* bg) {
	if (maxIndex > MAX_BG_COUNT) {
		// Error
		return;
	}
	lib[maxIndex] = bg;
	maxIndex++;
}