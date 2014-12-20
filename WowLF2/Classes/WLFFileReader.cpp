#include "AEroEngine.h"
#include "WLFAnimation.h"
#include "WLFFileReader.h"

extern ID3D11Device*						g_pd3dDevice;
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
	else if (resTypeStr == "5x1") {
		return RES_5x1;
	}
	else {
		return RES_1x1;
	}
}


VOID WLFDataFileReader::readResources(std::string fileName) {

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


VOID WLFDataFileReader::readObject(std::string fileName, AEObject* obj) {

	std::ifstream fs(fileName);
	if (fs.fail()) {
		AENSGameControl::exitGame("Cannot open file" + fileName);
	}

	AEFrame* frames[500];
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
				if (line == "#Animations_End")
					break;
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
									else if (item == "spellid:") {
										iss >> item;  attackJudge->spellID = stoi(item);
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
							else if (item == "$CreateSprite") {
								WLFSpriteCreatePoint* spriteCreate = new WLFSpriteCreatePoint;
								iss >> item;  spriteCreate->oid = stoi(item);
								spriteCreate->descSprite.obj = nullptr;
								spriteCreate->descSprite.action = 0;
								spriteCreate->descSprite.cx = 0.0f;
								spriteCreate->descSprite.cy = 0.0f;
								spriteCreate->descSprite.flip = SpriteEffects_None;
								spriteCreate->descSprite.layerDepth = 0.0f;
								spriteCreate->descSprite.team = 0;
								iss >> item;
								INT frame = -1;  // Add SpriteCreate for every frame by default
								while (item != "$End") {
									if (item == "frame:") {
										iss >> item;  frame = stoi(item);
									}
									else if (item == "action:") {
										iss >> item;  spriteCreate->descSprite.action = stoi(item);
									}
									else if (item == "team:") {
										iss >> item;  spriteCreate->descSprite.team = stoi(item);
									}
									else if (item == "flip:") {
										iss >> item;  spriteCreate->flip = stoi(item);
									}
									else if (item == "at:") {
										iss >> item;  spriteCreate->x = stoi(item);
										iss >> item;  spriteCreate->y = stoi(item);
									}
									else if (item == "depth:") {
										iss >> item;  spriteCreate->descSprite.layerDepth = stof(item);
									}
									else if (item == "scale:") {
										iss >> item;  spriteCreate->descSprite.scale = stof(item);
									}
									else if (item == "count:") {
										iss >> item;  spriteCreate->count = stoi(item);
									}
									iss >> item;
								}
								if (frame == -1) {
									for (INT i = 0; i < anim->getFrameCount(); i++) {
										anim->addSpriteCreateForFrame(i, spriteCreate);
									}
								}
								else {
									anim->addSpriteCreateForFrame(frame, spriteCreate);
								}
							}
							else if (item == "$Turn") {
								anim->setTurnAfterAnim(TRUE);
								iss >> item;
								while (item != "$End") {
									iss >> item;
								}
							}
							else if (item == "$CameraShake") {
								WLFCameraShakeOptions* cameraShake = new WLFCameraShakeOptions;
								iss >> item;  cameraShake->time = stoi(item);
								INT frame = 0;
								while (item != "$End") {
									if (item == "frame:") {
										iss >> item;  frame = stoi(item);
									}
									else if (item == "amplitude:") {
										iss >> item;  cameraShake->amplitude = stoi(item);
									}
									iss >> item;
								}
								anim->addCameraShakeForFrame(frame, cameraShake);
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