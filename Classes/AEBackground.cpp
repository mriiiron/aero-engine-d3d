#include <d3d11_1.h>
#include <string>
#include "AEMath.h"
#include "AEResource.h"
#include "AETable.h"
#include "AEBackground.h"

extern AEConstantTable<AEResource>			resourceTable;


AEBGLayerAnim::~AEBGLayerAnim() {
	delete [] frames;
	delete [] endTimes;
}


AEBackground::AEBackground(AERO_BACKGROUND_DESC desc) {
	name = desc.name;
	locX = desc.locX;  locY = desc.locY;
	width = desc.width;  height = desc.height;
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

VOID AEBackground::loadAELandformsFromMonochrome(BYTE* pixels, INT width, INT height, INT byteLine) {

}

VOID AEBackground::loadAELandforms(BYTE* pixels, INT width, INT height) {

}

VOID AEBackground::addAnimAt(INT index, AEBGLayerAnim layerAnim) {

}

VOID AEBackground::update() {
	for (INT i = 0; i < layerCount; i++) {
		for (INT j = 0; j < layerTable[i]->getAnimCount(); j++) {
			layerTable[i]->incTimeForAnim(j);
			AEBGLayerAnim* anim = animTable[layerTable[i]->getAnimRef(j).animIndex];
			if (layerTable[i]->getTimeOfAnim(j) >= anim->getEndTime(layerTable[i]->getFrameOfAnim(j))) {
				layerTable[i]->incFrameForAnim(j);
				if (layerTable[i]->getFrameOfAnim(j) == anim->getFrameCount()) {
					layerTable[i]->resetAnim(j);
				}
			}
		}
	}
}

VOID AEBackground::addToRenderBuffer(AEPoint cameraCenter) {
	for (INT i = layerCount - 1; i >= 0; i--) {
		FLOAT dx = cameraCenter.x;
		FLOAT depth = FLOAT(layerTable[i]->getDepth());
		FLOAT correction = dx * depth / 100.0f;
		for (INT j = 0; j < layerTable[i]->getAnimCount(); j++) {
			AEAnimRef ref = layerTable[i]->getAnimRef(j);
			AEBGLayerAnim* lanim = animTable[ref.animIndex];
			AEBGLayerFrame lf = lanim->getFrame(ref.frame);
			AEResource* res = resourceTable.getItem(lf.rid);
			AERect animRect;
			animRect.x1 = locX + layerTable[i]->getOffsetPosition().x + ref.x + correction;
			animRect.x2 = animRect.x1 + lf.width;
			animRect.y1 = locY + layerTable[i]->getOffsetPosition().y + ref.y;
			animRect.y2 = animRect.y1 + lf.height;
			AERect texClip = res->getTexClip(lf.imgOffset, 1);
			res->addToRenderBuffer(animRect, texClip);
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