#include <d3d11_1.h>

#include "AETable.h"
#include "AEResource.h"
#include "AEObject.h"
#include "AEPlatform.h"
#include "AEBackground.h"
#include "AEFileReader.h"

#include "DDSTextureLoader.h"

extern ID3D11Device*                        g_pd3dDevice;
extern AEConstantTable<AEResource>          ae_ResourceTable;
extern AEConstantTable<AEObject>            ae_ObjectTable;
extern AEConstantTable<AEBackground>        ae_BackgroundTable;


AEResType AEDataFileReader::resourceTypeDecrypt(std::string resTypeStr) {
    if (resTypeStr == "5x10") {
        return RES_5x10;
    }
    else if (resTypeStr == "4x5") {
        return RES_4x5;
    }
    else if (resTypeStr == "2x10") {
        return RES_2x10;
    }
    else if (resTypeStr == "2x5") {
        return RES_2x5;
    }
    else if (resTypeStr == "1x10") {
        return RES_1x10;
    }
    else if (resTypeStr == "1x5") {
        return RES_1x5;
    }
    else if (resTypeStr == "5x1") {
        return RES_5x1;
    }
    else if (resTypeStr == "1x1") {
        return RES_1x1;
    }
    else {
        AENSGameControl::exitGame("Illegal resource layout \"" + resTypeStr + "\".");
    }
}


VOID AEDataFileReader::loadResources(std::string fileName) {

    std::ifstream fs(fileName);
    if (fs.fail()) {
        AENSGameControl::exitGame("Cannot open file: " + fileName);
    }

    AERO_RESOURCE_DESC descRes;

    size_t newSizeW, convertedChars;

    std::string line, item;
    std::istringstream iss;
    while (!fs.eof()) {
        getline(fs, line);  iss.clear();  iss.str(line);
        if (line == "") continue;
        iss >> item;  descRes.rid = stoi(item);
        iss >> item;  descRes.rtype = resourceTypeDecrypt(item);
        iss >> item;  descRes.cellW = stoi(item);
        iss >> item;  descRes.cellH = stoi(item);
        iss >> item;
        newSizeW = strlen(item.c_str()) + 1;
        convertedChars = 0;
        wchar_t* witem = new wchar_t[newSizeW];
        mbstowcs_s(&convertedChars, witem, newSizeW, item.c_str(), _TRUNCATE);  // Must convert std::string to wchar_t*
        HRESULT hr = CreateDDSTextureFromFile(g_pd3dDevice, witem, nullptr, &(descRes.tex));
        if (FAILED(hr)) {
            AENSGameControl::exitGame("On loading texture: Texture load failed.");
        }
        ae_ResourceTable.addAt(descRes.rid, new AEResource(descRes));
    }

    fs.close();

}


AEFromTo AEDataFileReader::framesFromToDecrypt(std::string framesFromTo) {
    size_t delimPos = framesFromTo.find('-');
    if (delimPos == std::string::npos) {
        INT frameNum = stoi(framesFromTo);
        AEFromTo result = { frameNum, frameNum };
        return result;
    }
    else {
        INT from = stoi(framesFromTo.substr(0, delimPos));
        INT to = stoi(framesFromTo.substr(delimPos + 1, std::string::npos));
        AEFromTo result = { from, to };
        return result;
    }
}

VOID AEDataFileReader::loadObject(INT oid, std::string fileName) {

    std::ifstream fs(fileName);
    if (fs.fail()) {
        AENSGameControl::exitGame("Cannot open file" + fileName);
    }

    AEFrame* frames[AEObject::MAX_FRAME_COUNT];
    for (int i = 0; i < AEObject::MAX_FRAME_COUNT; i++) {
        frames[i] = nullptr;
    }

    AERO_OBJECT_DESC descObj;
    descObj.oid = oid;
    AEObject* obj = nullptr;

    AERO_FRAME_DESC descFrame;
    AERO_ANIMATION_DESC descAnim;

    std::string line, item;
    std::istringstream iss;
    while (!fs.eof()) {
        getline(fs, line);  iss.clear();  iss.str(line);
        if (line == "") continue;
        if (line == "#Object_Head") {
            while (TRUE) {
                getline(fs, line);  iss.clear();  iss.str(line);
                if (line == "") continue;
                if (line == "#Object_Head_End") break;
                iss >> item;
                if (item == "Name:") {
                    iss >> item;  descObj.name = item;
                }
                else if (item == "Type:") {
                    iss >> item;  descObj.type = stoi(item);
                }
            }
            obj = new AEObject(descObj);
        }
        else if (line == "#Frames") {
            while (TRUE) {
                getline(fs, line);  iss.clear();  iss.str(line);
                if (line == "") continue;
                if (line == "#Frames_End") break;
                iss >> item;
                if (item == "$Frame") {
                    AEFrame* frame = nullptr;
                    iss >> item;  INT frameNum = stoi(item);
                    getline(fs, line);  iss.clear();  iss.str(line);
                    iss >> item;
                    if (item == "$Image") {
                        iss >> item;  descFrame.res = ae_ResourceTable.getItem(stoi(item));
                        iss >> item;  descFrame.imgOffset = stoi(item);
                        iss >> item;  descFrame.imgCells = stoi(item);
                        iss >> item;
                        if (item == "center") {
                            iss >> item;  descFrame.centerx = stoi(item);
                            iss >> item;  descFrame.centery = stoi(item);
                        }
                        else {
                            AENSGameControl::exitGame("Error in \"" + fileName + "\":\nExpecting \"center\".");
                        }
                        frame = new AEFrame(descFrame);
                        while (TRUE) {
                            getline(fs, line);  iss.clear();  iss.str(line);
                            if (line == "$Frame_End") break;
                            iss >> item;
                            if (item == "$AttachSlot") {
                                iss >> item;  INT slotIndex = stoi(item);
                                iss >> item;  INT x = stoi(item);
                                iss >> item;  INT y = stoi(item);
                                frame->addAttachSlot(slotIndex, x, y);
                            }
                            else {
                                AENSGameControl::exitGame("Error in \"" + fileName + "\":\nUnexpected frame options \"" + item + "\".");
                            }
                        }
                    }
                    else {
                        AENSGameControl::exitGame("Error in \"" + fileName + "\":\nExpecting \"$Image\".");
                    }
                    frames[frameNum] = frame;
                }
            }
        }
        else if (line == "#Animations") {
            if (obj == nullptr) {
                AENSGameControl::exitGame("Object in " + fileName + " has not been initiated.");
            }
            while (TRUE) {
                getline(fs, line);  iss.clear();  iss.str(line);
                if (line == "") continue;
                if (line == "#Animations_End") break;
                iss >> item;
                if (item == "$Anim") {
                    iss >> item;  INT animNum = stoi(item);
                    iss >> item;  descAnim.name = item;
                    getline(fs, line);  iss.clear();  iss.str(line);
                    iss >> item;
                    if (item == "$FrameCount") {
                        iss >> item;  descAnim.frameCount = stoi(item);
                        iss >> item;  descAnim.isAnimLoop = (item == "loop" ? TRUE : FALSE);
                        iss >> item;  descAnim.next = stoi(item);  // 1000 -> ACTION_NUM_DEAD
                        AEAnimation* anim = new AEAnimation(descAnim);
                        INT* frameNums = new INT[descAnim.frameCount];
                        INT* endTimes = new INT[descAnim.frameCount];
                        // INT* xShifts = new INT[descAnim.frameCount];
                        getline(fs, line);  iss.clear();  iss.str(line);
                        for (INT i = 0; i < descAnim.frameCount; i++) {
                            iss >> item;  INT frameNum = stoi(item);
                            if (frames[frameNum] == nullptr) {
                                AENSGameControl::exitGame("Error in " + fileName + ":\nAnimation " + std::to_string(animNum) + " uses an inexistent frame " + std::to_string(frameNum) + ".");
                            }
                            frameNums[i] = frameNum;
                        }
                        getline(fs, line);  iss.clear();  iss.str(line);
                        INT endTime = 0;
                        for (INT i = 0; i < descAnim.frameCount; i++) {
                            iss >> item;  endTime += stoi(item);  endTimes[i] = endTime;
                        }
                        for (INT i = 0; i < descAnim.frameCount; i++) {
                            anim->addFrame(i, frames[frameNums[i]], endTimes[i]);
                        }
                        while (TRUE) {
                            getline(fs, line);  iss.clear();  iss.str(line);
                            if (line == "$Anim_End") break;
                            iss >> item;
                            if (item == "$TTL") {
                                iss >> item;  anim->setTTL(stoi(item));
                            }
                            else {
                                AENSGameControl::exitGame("Error in \"" + fileName + "\":\nUnexpected animation option(s).");
                            }
                        }
                        delete [] frameNums;
                        delete [] endTimes;
                        obj->addAnimationAt(animNum, anim);
                    }
                    else {
                        AENSGameControl::exitGame("Error in \"" + fileName + "\":\nExpecting \"$FrameCount\".");
                    }
                }
                else {
                    AENSGameControl::exitGame("Error in \"" + fileName + "\":\nExpecting \"$Anim\".");
                }
            }
        }
    }

    ae_ObjectTable.addAt(oid, obj);
    fs.close();

}

VOID AEDataFileReader::loadBackground(INT bgid, std::string fileName) {

    std::string loadingQueue[5] = { "#BG_Head", "#Frames", "#Animations", "#Layers", "#Platforms" };
    INT loadingNum = 0;

    std::ifstream fs(fileName);
    if (fs.fail()) {
        AENSGameControl::exitGame("Cannot open file" + fileName);
    }

    AEFrame* frames[AEObject::MAX_FRAME_COUNT];
    for (int i = 0; i < AEObject::MAX_FRAME_COUNT; i++) {
        frames[i] = nullptr;
    }

    AERO_BACKGROUND_DESC descBG;
    descBG.bgid = bgid;

    AEBackground* bg = nullptr;

    AERO_FRAME_DESC descFrame;
    AERO_ANIMATION_DESC descAnim;
    AERO_BG_ELEMENT_DESC descElem;
    AERO_BG_LAYER_DESC descLayer;
    AERO_PLATFORM_DESC descPlatform;

    std::string line, item;
    std::istringstream iss;
    while (!fs.eof()) {
        getline(fs, line);  iss.clear();  iss.str(line);
        if (line == "") continue;
        if (line == "#BG_Head") {
            if (line == loadingQueue[loadingNum]) {
                loadingNum++;
            }
            else {
                AENSGameControl::exitGame("Error in \"" + fileName + "\":\nExpecting \"" + loadingQueue[loadingNum] + "\".");
            }
            while (TRUE) {
                getline(fs, line);  iss.clear();  iss.str(line);
                if (line == "") continue;
                if (line == "#BG_Head_End") break;
                iss >> item;
                if (item == "Name:") {
                    iss >> item;  descBG.name = item;
                }
            }
            bg = new AEBackground(descBG);
        }
        else if (line == "#Frames") {
            if (line == loadingQueue[loadingNum]) {
                loadingNum++;
            }
            else {
                AENSGameControl::exitGame("Error in \"" + fileName + "\":\nExpecting \"" + loadingQueue[loadingNum] + "\".");
            }
            while (TRUE) {
                getline(fs, line);  iss.clear();  iss.str(line);
                if (line == "") continue;
                if (line == "#Frames_End") break;
                iss >> item;
                if (item == "$Frame") {
                    AEFrame* frame = nullptr;
                    iss >> item;  INT frameNum = stoi(item);
                    getline(fs, line);  iss.clear();  iss.str(line);
                    iss >> item;
                    if (item == "$Image") {
                        iss >> item;  descFrame.res = ae_ResourceTable.getItem(stoi(item));
                        iss >> item;  descFrame.imgOffset = stoi(item);
                        iss >> item;  descFrame.imgCells = stoi(item);
                        iss >> item;
                        if (item == "center") {
                            iss >> item;  descFrame.centerx = stoi(item);
                            iss >> item;  descFrame.centery = stoi(item);
                        }
                        else {
                            AENSGameControl::exitGame("Error in \"" + fileName + "\":\nExpecting \"center\".");
                        }
                        frame = new AEFrame(descFrame);
                        while (TRUE) {
                            getline(fs, line);  iss.clear();  iss.str(line);
                            if (line == "$Frame_End") break;
                            //iss >> item;
                            //if (item == "$...") {

                            //}
                            //else {
                            //    AENSGameControl::exitGame("Error in \"" + fileName + "\":\nUnexpected frame options \"" + item + "\".");
                            //}
                        }
                    }
                    else {
                        AENSGameControl::exitGame("Error in \"" + fileName + "\":\nExpecting \"$Image\".");
                    }
                    frames[frameNum] = frame;
                }
            }
        }
        else if (line == "#Animations") {
            if (line == loadingQueue[loadingNum]) {
                loadingNum++;
            }
            else {
                AENSGameControl::exitGame("Error in \"" + fileName + "\":\nExpecting \"" + loadingQueue[loadingNum] + "\".");
            }
            while (TRUE) {
                getline(fs, line);  iss.clear();  iss.str(line);
                if (line == "") continue;
                if (line == "#Animations_End") break;
                iss >> item;
                if (item == "$Anim") {
                    iss >> item;  INT animNum = stoi(item);
                    iss >> item;  descAnim.name = item;
                    getline(fs, line);  iss.clear();  iss.str(line);
                    iss >> item;
                    if (item == "$FrameCount") {
                        iss >> item;  descAnim.frameCount = stoi(item);
                        iss >> item;  descAnim.isAnimLoop = (item == "loop" ? TRUE : FALSE);
                        iss >> item;  descAnim.next = stoi(item);  // 1000 -> ACTION_NUM_DEAD
                        AEAnimation* anim = new AEAnimation(descAnim);
                        INT* frameNums = new INT[descAnim.frameCount];
                        INT* endTimes = new INT[descAnim.frameCount];
                        getline(fs, line);  iss.clear();  iss.str(line);
                        for (INT i = 0; i < descAnim.frameCount; i++) {
                            iss >> item;  INT frameNum = stoi(item);
                            if (frames[frameNum] == nullptr) {
                                AENSGameControl::exitGame("Error in " + fileName + ":\nAnimation " + std::to_string(animNum) + " uses an inexistent frame " + std::to_string(frameNum) + ".");
                            }
                            frameNums[i] = frameNum;
                        }
                        getline(fs, line);  iss.clear();  iss.str(line);
                        INT endTime = 0;
                        for (INT i = 0; i < descAnim.frameCount; i++) {
                            iss >> item;  endTime += stoi(item);  endTimes[i] = endTime;
                        }
                        for (INT i = 0; i < descAnim.frameCount; i++) {
                            anim->addFrame(i, frames[frameNums[i]], endTimes[i]);
                        }
                        while (TRUE) {
                            getline(fs, line);  iss.clear();  iss.str(line);
                            if (line == "$Anim_End") break;
                        }
                        delete[] frameNums;
                        delete[] endTimes;
                        bg->addAnimationAt(animNum, anim);
                    }
                    else {
                        AENSGameControl::exitGame("Error in \"" + fileName + "\":\nExpecting \"$FrameCount\".");
                    }
                }
                else {
                    AENSGameControl::exitGame("Error in \"" + fileName + "\":\nExpecting \"$Anim\".");
                }
            }
        }
        else if (line == "#Layers") {
            if (line == loadingQueue[loadingNum]) {
                loadingNum++;
            }
            else {
                AENSGameControl::exitGame("Error in \"" + fileName + "\":\nExpecting \"" + loadingQueue[loadingNum] + "\".");
            }
            while (TRUE) {
                getline(fs, line);  iss.clear();  iss.str(line);
                if (line == "") continue;
                if (line == "#Layers_End") break;
                iss >> item;
                if (item == "$Layer") {
                    INT layerIndex;
                    iss >> item;  layerIndex = stoi(item);
                    iss >> item;  descLayer.name = item;
                    getline(fs, line);  iss.clear();  iss.str(line);
                    iss >> item;
                    if (item == "$Depth") {
                        iss >> item;  descLayer.depth = stoi(item);
                    }
                    else {
                        AENSGameControl::exitGame("Error in \"" + fileName + "\":\nExpecting \"$Depth\".");
                    }
                    AEBGLayer* layer = new AEBGLayer(descLayer);
                    while (TRUE) {
                        getline(fs, line);  iss.clear();  iss.str(line);
                        if (line == "$Layer_End") break;
                        iss >> item;
                        if (item == "$Element") {
                            descElem.alpha = 1.0f;
                            iss >> item;  descElem.anim = bg->getAnimation(stoi(item));
                            iss >> item;  descElem.cx = stoi(item);
                            iss >> item;  descElem.cy = stoi(item);
                            iss >> item;
                            while (item != "$End") {
                                if (item == "alpha:") {
                                    iss >> item;  descElem.alpha = stof(item);
                                }
                                else {
                                    AENSGameControl::exitGame("Error in \"" + fileName + "\":\nUnexpected $Element option(s).");
                                }
                                iss >> item;
                            }
                            AEBGLayerElement* elem = new AEBGLayerElement(descElem);
                            layer->addElement(elem);
                        }
                        else {
                            AENSGameControl::exitGame("Error in \"" + fileName + "\":\nExpecting \"$Element\".");
                        }
                    }
                    bg->addLayerAt(layerIndex, layer);
                }
                else {
                    AENSGameControl::exitGame("Error in \"" + fileName + "\":\nExpecting \"$Layer\".");
                }
            }
        }
        else if (line == "#Platforms") {
            if (line == loadingQueue[loadingNum]) {
                loadingNum++;
            }
            else {
                AENSGameControl::exitGame("Error in \"" + fileName + "\":\nExpecting \"" + loadingQueue[loadingNum] + "\".");
            }
            while (TRUE) {
                getline(fs, line);  iss.clear();  iss.str(line);
                if (line == "") continue;
                if (line == "#Platforms_End") break;
                iss >> item;
                if (item == "$Platform") {
                    iss >> item;  descPlatform.pid = stoi(item);
                    getline(fs, line);  iss.clear();  iss.str(line);
                    iss >> item;
                    if (item == "$NodeCount") {
                        iss >> item;  descPlatform.nodeCount = stoi(item);
                    }
                    else {
                        AENSGameControl::exitGame("Error in \"" + fileName + "\":\nExpecting \"$NodeCount\".");
                    }
                    AEPlatform* platform = new AEPlatform(descPlatform);
                    for (INT i = 0; i < descPlatform.nodeCount; i++) {
                        getline(fs, line);  iss.clear();  iss.str(line);
                        iss >> item;  FLOAT nodeX = stof(item);
                        iss >> item;  FLOAT nodeY = stof(item);
                        platform->addNode({ nodeX, nodeY });
                    }
                    getline(fs, line);  iss.clear();  iss.str(line);
                    if (line != "$Platform_End") {
                        AENSGameControl::exitGame("Error in \"" + fileName + "\":\nError number of platform nodes on platform # " + std::to_string(descPlatform.pid) + ".");
                    }
                    platform->calcAngles();
                    bg->addPlatform(platform); 
                }
                else {
                    AENSGameControl::exitGame("Error in \"" + fileName + "\":\nExpecting \"$Platform\".");
                }
            }
        }
    }

    ae_BackgroundTable.addAt(bgid, bg);
    fs.close();

}