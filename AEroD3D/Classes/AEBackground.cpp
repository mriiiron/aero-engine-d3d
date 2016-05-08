#include <d3d11_1.h>
#include <string>
#include "AEMath.h"
#include "AEResource.h"
#include "AETable.h"
#include "AEAnimation.h"
#include "AEPlatform.h"
#include "AEBackground.h"

#include "SpriteBatch.h"

extern SpriteBatch*							xtk_SpriteBatch;


AEBGLayerElement::AEBGLayerElement(AERO_BG_ELEMENT_DESC desc) {
    cx = desc.cx;
    cy = desc.cy;
    anim = desc.anim;
    alpha = desc.alpha;
    time = frameNum = 0;
}

VOID AEBGLayerElement::update() {
    time++;
    if (time >= anim->getEndTime(frameNum)) {
        frameNum++;
        if (time >= anim->getEndTime(anim->getFrameCount() - 1)) {
            time = 0;
        }
        if (frameNum == anim->getFrameCount()) {
            frameNum = 0;
        }
    }
}

VOID AEBGLayerElement::render(FLOAT correction) {
    AEFrame* f = anim->getFrame(frameNum);
    FLOAT fwidth = (FLOAT)(f->getWidth()), fcenterx = (FLOAT)(f->getCenter().x), fcentery = (FLOAT)(f->getCenter().y);
    AEResource* res = f->getResource();
    RECT texClipInTexel = res->getTexClipInTexel(f->getImgOffset(), f->getImgCells());
    xtk_SpriteBatch->Draw(
        res->getTexture(), // Texture
        XMFLOAT2((INT)(cx + correction), (INT)cy), // Drawing Destination Position (Origin Point)
        &texClipInTexel, // Texture Clip Rectangle
        XMVectorSet(1.0f, 1.0f, 1.0f, alpha), // Tilting Color
        0.0f, // Rotation
        XMFLOAT2(fcenterx, fcentery), // Rotation Origin / Drawing Center
        1.0f, // Scale
        SpriteEffects::SpriteEffects_None, // Sprite Effects
        1.0f // Z Value
    );
}


AEBGLayer::AEBGLayer(AERO_BG_LAYER_DESC desc) {
    name = desc.name;
    depth = desc.depth;
    elemTable = new AEConstantTable<AEBGLayerElement>(MAX_LAYER_ELEMENTS);
}

VOID AEBGLayer::addElement(AEBGLayerElement* elem) {
    elemTable->add(elem);
}

VOID AEBGLayer::update() {
    for (INT i = 0; i < elemTable->getMaxElemCount(); i++) {
        AEBGLayerElement* elem = elemTable->getItem(i);
        if (elem != nullptr) {
            elem->update();
        }
    }
}

VOID AEBGLayer::render(FLOAT correction) {
    for (INT i = 0; i < elemTable->getMaxElemCount(); i++) {
        AEBGLayerElement* elem = elemTable->getItem(i);
        if (elem != nullptr) {
            elem->render(correction);
        }
    }
}


AEBackground::AEBackground(AERO_BACKGROUND_DESC desc) {
    bgid = desc.bgid;
    name = desc.name;
    layerTable = new AEConstantTable<AEBGLayer>(MAX_LAYER_COUNT);
    platformTable = new AEHashedTable<AEPlatform>(MAX_PLATFORM_COUNT);
    animTable = new AEConstantTable<AEAnimation>(MAX_ANIM_COUNT);
}

VOID AEBackground::update() {
    for (INT i = 0; i < layerTable->getMaxElemCount(); i++) {
        AEBGLayer* layer = layerTable->getItem(i);
        if (layer != nullptr) {
            layer->update();
        }
    }
}

VOID AEBackground::render(XMFLOAT2 ae_CameraCenter) {
    for (INT i = 0; i < layerTable->getMaxElemCount(); i++) {
        AEBGLayer* layer = layerTable->getItem(i);
        if (layer != nullptr) {
            FLOAT dx = ae_CameraCenter.x;
            FLOAT depth = FLOAT(layer->getDepth());
            FLOAT correction = dx * depth / 100.0f;
            layer->render(correction);
        }
    }
}