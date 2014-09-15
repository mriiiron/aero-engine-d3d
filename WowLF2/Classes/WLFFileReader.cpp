#include "AEroEngine.h"
#include "WLFAnimation.h"
#include "WLFFileReader.h"

extern AEConstantTable<AEResource>			ae_ResourceTable;


AEResType WLFDataFileReader::resourceTypeDecrypt(std::string resTypeStr) {
	if (resTypeStr == "5x10") {
		return RES_5x10;
	}
	else if (resTypeStr == "4x5") {
		return RES_4x5;
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
	else {
		return RES_1x1;
	}
}


VOID WLFDataFileReader::readResources(std::string fileName, AEConstantTable<AEResource>* pResourceTable, ID3D11Device** device) {

	std::ifstream fs(fileName);
	if (fs.fail()) {
		AENSGameControl::exitGame("Cannot open file" + fileName);
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
		iss >> item;  descRes.cellW = stoi(item);
		iss >> item;
		newSizeW = strlen(item.c_str()) + 1;
		convertedChars = 0;
		wchar_t* witem = new wchar_t[newSizeW];
		mbstowcs_s(&convertedChars, witem, newSizeW, item.c_str(), _TRUNCATE);  // Must convert std::string to wchar_t*
		HRESULT hr = CreateDDSTextureFromFile(*device, witem, nullptr, &(descRes.tex));
		if (FAILED(hr)) {
			AENSGameControl::exitGame("On loading texture: Texture load failed.");
		}
		pResourceTable->addAt(descRes.rid, new AEResource(descRes));
	}

	fs.close();

}


VOID WLFDataFileReader::readObject(std::string fileName, AEObject* obj) {

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
			INT frameNumOffset = 0;
			while (TRUE) {
				getline(fs, line);  iss.clear();  iss.str(line);
				if (line == "") continue;
				if (line == "#Frames_End") break;
				iss >> item;
				if (item == "$ImageResource") {
					iss >> item;  descFrame.res = ae_ResourceTable.getItem(stoi(item));
					while (TRUE) {
						getline(fs, line);  iss.clear();  iss.str(line);
						if (line == "$ImageResource_End") {
							frameNumOffset += descFrame.res->getCellCount();
							break;
						}
						iss >> item;  INT frameNum = frameNumOffset + stoi(item);  descFrame.imgOffset = stoi(item);
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
						iss >> item;  descAnim.next = stoi(item);  // 1000 -> ACTION_NUM_DEAD
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
							iss >> item;
							if (item == "$Attack") {
								WLFAttackJudgeArea* attackJudge = new WLFAttackJudgeArea;
								iss >> item;  attackJudge->rect.left = stoi(item);
								iss >> item;  attackJudge->rect.top = stoi(item);
								iss >> item;  attackJudge->rect.right = stoi(item);
								iss >> item;  attackJudge->rect.bottom = stoi(item);
								iss >> item;
								INT frame = -1;
								while (item != "$End") {
									if (item == "frame:") {
										iss >> item;  frame = stoi(item);
									}
									else if (item == "effect:") {
										iss >> item;  attackJudge->effect = stoi(item);
									}
									else if (item == "angle:") {
										iss >> item;  attackJudge->angle = stoi(item);
									}
									iss >> item;
								}
								if (frame == -1) {
									for (INT i = 0; i < anim->getFrameCount(); i++) {
										anim->addAttackJudgeForFrame(i, attackJudge);
									}
								}
								else {
									anim->addAttackJudgeForFrame(frame, attackJudge);
								}
							}
							else if (item == "$Body") {
								WLFBodyJudgeArea* bodyJudge = new WLFBodyJudgeArea;
								iss >> item;  bodyJudge->rect.left = stoi(item);
								iss >> item;  bodyJudge->rect.top = stoi(item);
								iss >> item;  bodyJudge->rect.right = stoi(item);
								iss >> item;  bodyJudge->rect.bottom = stoi(item);
								iss >> item;
								INT frame = -1;
								while (item != "$End") {
									if (item == "frame:") {
										iss >> item;  frame = stoi(item);
									}
								}
								if (frame == -1) {
									for (INT i = 0; i < anim->getFrameCount(); i++) {
										anim->addBodyJudgeForFrame(i, bodyJudge);
									}
								}
								else {
									anim->addBodyJudgeForFrame(frame, bodyJudge);
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