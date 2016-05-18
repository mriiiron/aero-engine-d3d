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


struct AERO_BG_ELEMENT_DESC {

    INT cx, cy;
    AEAnimation* anim;
    FLOAT alpha;

    AERO_BG_ELEMENT_DESC() {
        cx = cy = 0;
        anim = nullptr;
        alpha = 1.0f;
    }

};

class AEBGLayerElement {

public:

    AEBGLayerElement(AERO_BG_ELEMENT_DESC desc);

    virtual VOID update();
    virtual VOID render(FLOAT correction);

protected:

    INT cx, cy;
    INT time, frameNum;
    AEAnimation* anim;
    FLOAT alpha;

};


struct AERO_BG_LAYER_DESC {
    
    std::string name;
    INT depth;

    AERO_BG_LAYER_DESC() {
        name = "Unnamed Layer";
        depth = 0;
    }

};


class AEBGLayer {

public:

    static const INT MAX_LAYER_ELEMENTS			= 500;

    AEBGLayer(AERO_BG_LAYER_DESC desc);

    INT getDepth() { return depth; }
    AEBGLayerElement* getElem(INT index) { return elemTable->getItem(index); }

    VOID addElement(AEBGLayerElement* elem);
    
    VOID update();
    VOID render(FLOAT correction);

private:

    std::string name;
    INT depth;
    AEConstantTable<AEBGLayerElement>* elemTable;

};


struct AERO_BACKGROUND_DESC {

    INT bgid;
    std::string name;

    AERO_BACKGROUND_DESC() {
        bgid = 0;
        name = "Unknown Background";
    }

};


class AEBackground {

public:

    static const INT MAX_LAYER_COUNT			= 100;
    static const INT MAX_ANIM_COUNT             = 500;
    static const INT MAX_PLATFORM_COUNT         = 500;
    static const INT ON_LANDFORM_TOLERANCE		= 2;

    AEBackground(AERO_BACKGROUND_DESC desc);

    VOID addLayerAt(INT index, AEBGLayer* layer) { layerTable->addAt(index, layer); }
    VOID addAnimationAt(INT index, AEAnimation* anim) { animTable->addAt(index, anim); }
    VOID addPlatform(AEPlatform* platform) { platformTable->add(platform); }
    AEAnimation* getAnimation(INT index) { return animTable->getItem(index); }
    AEHashedTable<AEPlatform>* getPlatformTable() { return platformTable; }

    VOID update();
    VOID render(XMFLOAT2 ae_CameraCenter);

private:

    std::string name;
    INT bgid;
    AEConstantTable<AEAnimation>* animTable;
    AEConstantTable<AEBGLayer>* layerTable;
    AEHashedTable<AEPlatform>* platformTable;

};