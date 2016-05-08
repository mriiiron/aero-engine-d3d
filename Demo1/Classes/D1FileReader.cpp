#include "AEroEngine.h"

#include "D1Animation.h"
#include "D1FileReader.h"


extern AEConstantTable<AEResource>          ae_ResourceTable;
extern AEConstantTable<AEObject>            ae_ObjectTable;


VOID D1DataFileReader::loadObject(INT oid, std::string fileName)
{

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
            INT frameNumOffset = 0;
            while (TRUE) {
                getline(fs, line);  iss.clear();  iss.str(line);
                if (line == "") continue;
                if (line == "#Frames_End") break;
                iss >> item;
                if (item == "$Frame") {
                    D1Frame* frame = nullptr;
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
                        frame = new D1Frame(descFrame);
                        while (TRUE) {
                            getline(fs, line);  iss.clear();  iss.str(line);
                            if (line == "$Frame_End") break;
                            iss >> item;
                            if (item == "$Body") {
                                iss >> item;  INT x1 = stoi(item);
                                iss >> item;  INT y1 = stoi(item);
                                iss >> item;  INT x2 = stoi(item);
                                iss >> item;  INT y2 = stoi(item);
                                frame->setBdyJudge(new AERect(x1, y1, x2, y2));
                            }
                            else if (item == "$Attack") {
                                iss >> item;  INT x1 = stoi(item);
                                iss >> item;  INT y1 = stoi(item);
                                iss >> item;  INT x2 = stoi(item);
                                iss >> item;  INT y2 = stoi(item);
                                frame->setAtkJudge(new AERect(x1, y1, x2, y2));
                            }
                            else if (item == "$AttachSlot") {
                                iss >> item;  INT slotNum = stoi(item);
                                iss >> item;  INT x = stoi(item);
                                iss >> item;  INT y = stoi(item);
                                frame->addAttachSlot(slotNum, x, y);
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
                        D1Animation* anim = new D1Animation(descAnim);
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
                            iss >> item;
                            if (item == "$CreateSprite") {
                                D1AnimOption_CreateSprite* spriteCreate = new D1AnimOption_CreateSprite(); 
                                iss >> item;
                                INT frame = -1;  // Add SpriteCreate for every frame by default
                                while (item != "$End") {
                                    if (item == "object:") {
                                        iss >> item;  spriteCreate->oid = stoi(item);
                                    }
                                    else if (item == "action:") {
                                        iss >> item;  spriteCreate->descSprite.action = stoi(item);
                                    }
                                    else if (item == "at:") {
                                        iss >> item;  spriteCreate->x = stoi(item);
                                        iss >> item;  spriteCreate->y = stoi(item);
                                    }
                                    else if (item == "frame:") {
                                        iss >> item;  frame = stoi(item);
                                    }
                                    else if (item == "speed:") {
                                        iss >> item;  spriteCreate->vx = stof(item);
                                        iss >> item;  spriteCreate->vy = stof(item);
                                    }
                                    else {
                                        AENSGameControl::exitGame("Error in \"" + fileName + "\":\nUnexpected $CreateSprite option(s).");
                                    }
                                    iss >> item;
                                }
                                if (frame == -1) {
                                    for (INT i = 0; i < anim->getFrameCount(); i++) {
                                        anim->addSpriteCreationForFrame(i, spriteCreate);
                                    }
                                }
                                else {
                                    anim->addSpriteCreationForFrame(frame, spriteCreate);
                                }
                            }
                            else if (item == "$TTL") {
                                iss >> item;  anim->setTTL(stoi(item));
                            }
                            else {
                                AENSGameControl::exitGame("Error in \"" + fileName + "\":\nUnexpected animation option(s).");
                            }
                        }
                        delete[] frameNums;
                        delete[] endTimes;
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
