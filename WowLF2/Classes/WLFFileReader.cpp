#include "AEroEngine.h"
#include "WLFAnimation.h"
#include "WLFFileReader.h"

extern AEConstantTable<AEResource>			ae_ResourceTable;


VOID WLFObjectDataFileReader::read(std::string fileName, AEObject* obj) {

	std::ifstream fs(fileName);
	if (fs.fail()) {
		AENSGameControl::exitGame("Cannot open file" + fileName);
	}

	AEFrame* frames[50];
	AERO_FRAME_DESC descFrame;
	AERO_ANIMATION_DESC descAnim;

	std::string line, item;
	std::istringstream iss;
	while (!fs.eof()) {
		getline(fs, line);  iss.clear();  iss.str(line);
		if (line == "") continue;
		if (line == "#Frames") {
			while (TRUE) {
				getline(fs, line);  iss.clear();  iss.str(line);
				if (line == "") continue;
				if (line == "#Frames_End") break;
				iss >> item;
				if (item == "$ImageResource") {
					iss >> item;  descFrame.res = ae_ResourceTable.getItem(stoi(item));
					while (TRUE) {
						getline(fs, line);  iss.clear();  iss.str(line);
						if (line == "$ImageResource_End") break;
						iss >> item;  INT frameNum = descFrame.imgOffset = stoi(item);
						iss >> item;  descFrame.centerx = stoi(item);
						iss >> item;  descFrame.centery = stoi(item);
						iss >> item;  descFrame.imgCells = item == "2-Cell" ? 2 : 1;
						frames[frameNum] = new AEFrame(descFrame);
					}
				}
			}
		}
		else if (line == "#Animations") {
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
						iss >> item;  descAnim.next = stoi(item);
						WLFAnimation* anim = new WLFAnimation(descAnim);
						INT* frameNums = new INT[descAnim.frameCount];
						INT* endTimes = new INT[descAnim.frameCount];
						INT* xShifts = new INT[descAnim.frameCount];
						getline(fs, line);  iss.clear();  iss.str(line);
						for (INT i = 0; i < descAnim.frameCount; i++) {
							iss >> item;  frameNums[i] = stoi(item);
						}
						getline(fs, line);  iss.clear();  iss.str(line);
						INT endTime = 0;
						for (INT i = 0; i < descAnim.frameCount; i++) {
							iss >> item;  endTime += stoi(item);  endTimes[i] = endTime;
						}
						getline(fs, line);  iss.clear();  iss.str(line);
						for (INT i = 0; i < descAnim.frameCount; i++) {
							iss >> item;  xShifts[i] = stoi(item);
						}
						for (INT i = 0; i < descAnim.frameCount; i++) {
							anim->addFrame(i, frames[frameNums[i]], endTimes[i]);
							anim->addXShiftForFrame(i, xShifts[i]);
						}
						while (TRUE) {
							getline(fs, line);  iss.clear();  iss.str(line);
							if (line == "$Anim_End") break;
							WLFJudgeAreaRect* jaRect = new WLFJudgeAreaRect;
							iss >> item;
							if (item == "$Attack") {
								jaRect->type = WLFAnimation::JUDGE_TYPE_ATTACK;
								iss >> item;  jaRect->rect.left = stoi(item);
								iss >> item;  jaRect->rect.top = stoi(item);
								iss >> item;  jaRect->rect.right = stoi(item);
								iss >> item;  jaRect->rect.bottom = stoi(item);
								if (item == "frame") {
									iss >> item;  anim->addJudgeAreaForFrame(stoi(item), jaRect);
								}
								else if (item == "all") {
									for (INT i = 0; i < anim->getFrameCount(); i++) {
										anim->addJudgeAreaForFrame(i, jaRect);
									}
								}
							}
							else if (item == "$On_Hit") {
								jaRect->type = WLFAnimation::JUDGE_TYPE_ON_HIT;
								iss >> item;  jaRect->rect.left = stoi(item);
								iss >> item;  jaRect->rect.top = stoi(item);
								iss >> item;  jaRect->rect.right = stoi(item);
								iss >> item;  jaRect->rect.bottom = stoi(item);
								if (item == "frame") {
									iss >> item;  anim->addJudgeAreaForFrame(stoi(item), jaRect);
								}
								else if (item == "all") {
									for (INT i = 0; i < anim->getFrameCount(); i++) {
										anim->addJudgeAreaForFrame(i, jaRect);
									}
								}
							}
						}
						delete[] frameNums;
						delete[] endTimes;
						delete[] xShifts;
						obj->addAnim(animNum, anim);
					}
				}
			}
		}
	}

	fs.close();

}