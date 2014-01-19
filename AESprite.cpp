#include <d3d11_1.h>
#include <string>
#include <cmath>
#include "AEMath.h"
#include "AEPhysics.h"
#include "AEResource.h"
#include "AEBackground.h"
#include "AEScene.h"
#include "AESprite.h"

extern AEResourceTable rTable;
extern AEObjectTable oTable;

AESprite::AESprite(AERO_SPRITE_DESC desc, AEScene* _sceneRef) {
	index = 0;
	oid = desc.oid;  team = desc.team;  cx = desc.cx;  cy = desc.cy;
	vx = vy = ax = ay = angle = vangle = gndSpeed = 0.0f;
	frameNum = time = facing = timeToStiff = keyState = atkJudgeLock = 0;
	sceneRef = _sceneRef;
	AEBackground* bg = sceneRef->getBackground();
	onLandform = (bg == NULL ? -1 : bg->getLandformIndexBelow(INT(cx), INT(cy), &drop));
	hpValue = hpMax = 100;
	if (desc.inverse)
		turnOver();
	if (oid >= 0)
		changeAction(desc.action);
	keyboardHandler = NULL;
}

AEVector2 AESprite::getFaceVector() {
	if (angle == 3.14159265f / 2) {
		return AEVector2(0.0f, 1.0f);
	}
	else if (angle == -3.14159265f / 2) {
		return AEVector2(0.0f, -1.0f);
	}
	else if (angle > -3.14159265f / 2 && angle < 3.14159265f / 2) {
		return AEVector2(1.0f, tan(angle));
	}
	else {
		return AEVector2(-1.0f, -tan(angle));
	}
}

AEPoint AESprite::calcRotatedPoint(AEPoint point, GLfloat cx, GLfloat cy, AEFrame* f, GLfloat angle, GLbyte facing) {
	GLfloat cosA = cos(angle), sinA = sin(angle);
	AEPoint rotated;
	if (facing == FACING_RIGHT) {
		rotated.x = cx + (point.x - f->getCenterx()) * cosA - (point.y - f->getCentery()) * sinA;
		rotated.y = cy + (point.x - f->getCenterx()) * sinA + (point.y - f->getCentery()) * cosA;
	}
	else {
		rotated.x = cx - (point.x - f->getCenterx()) * cosA + (point.y - f->getCentery()) * sinA;
		rotated.y = cy + (point.x - f->getCenterx()) * sinA + (point.y - f->getCentery()) * cosA;
	}
	return rotated;
}

AEBiasRect AESprite::calcRotatedRect(GLfloat cx, GLfloat cy, AEFrame* f, GLfloat angle, GLbyte facing) {
	GLfloat cosA = cos(angle), sinA = sin(angle);
	AEBiasRect bRect;
	INT centerx = f->getCenterx(), centery = f->getCentery();
	INT width = f->getWidth(), height = f->getHeight();
	if (facing == FACING_RIGHT) {
		bRect.x1 = cx - centerx * cosA + centery * sinA;
		bRect.y1 = cy - centerx * sinA - centery * cosA;
		bRect.x2 = bRect.x1 + width * cosA;
		bRect.y2 = bRect.y1 + width * sinA;
		bRect.x3 = bRect.x2 - height * sinA;
		bRect.y3 = bRect.y2 + height * cosA;
		bRect.x4 = bRect.x1 - height * sinA;
		bRect.y4 = bRect.y1 + height * cosA;
	}
	else {
		bRect.x2 = cx + centerx * cosA - centery * sinA;
		bRect.y2 = cy - centerx * sinA - centery * cosA;
		bRect.x1 = bRect.x2 - width * cosA;
		bRect.y1 = bRect.y2 + width * sinA;
		bRect.x3 = bRect.x2 + height * sinA;
		bRect.y3 = bRect.y2 + height * cosA;
		bRect.x4 = bRect.x1 + height * sinA;
		bRect.y4 = bRect.y1 + height * cosA;
	}
	return bRect;
}

string AESprite::getObjName() {
	return oTable.get(oid)->getName();
}

GLvoid AESprite::changeAction(INT _action) {
	if (_action == 1000) {
		scene->getSpriteTable()->remove(index);
		return;
	}
	action = _action;
	AEAnimation* anim = oTable.get(oid)->getAnim(action);
	state = anim->getState();
	timeToLive = anim->getTTL();
	frame = time = 0;
	if (state >= AEObject::STATE_CHAR_ACTION) {
		INT dvx = anim->getFrame(frame)->getDvx();
		if (dvx == 999)
			vx = 0;
		else
			vx += dvx;
		INT dvy = anim->getFrame(frame)->getDvy();
		if (dvy == 999)
			vy = 0;
		else
			vy += dvy;
	}
	else { 
		ax = vx = ay = vy = 0.0;
	}
	//if (anim->getFrame(frame).cast != NULL) {
	//	AEFrame* f = anim->getFrame(frame);
	//	AEPoint castPoint = calcRotatedPoint(cx, cy, &f, angle, facing);
	//	if (facing == 0)
	//		scene->getSpriteTable()->add(new AESprite(scene, f.cast->oid, team, castPoint.x, castPoint.y, f.cast->action));
	//	else
	//		scene->getSpriteTable()->add(new AESprite(scene, f.cast->oid, team, castPoint.x, castPoint.y, f.cast->action, CAST_INVERSE));
	//}
}

INT AESprite::log2(INT key) {
	switch (key) {
	case 0x1: return 0;
	case 0x2: return 1;
	case 0x4: return 2;
	case 0x8: return 3;
	case 0x10: return 4;
	case 0x20: return 5;
	case 0x40: return 6;
	default: return -1;
	}
}

//GLvoid AESprite::pressUp() {
//	keyDown(AEKeyboardHandler::AKS_UP);
//	inputState[AEKeyboardHandler::INPUT_8] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
//}
//
//GLvoid AESprite::pressDown() {
//	keyDown(AEKeyboardHandler::AKS_DOWN);
//	inputState[AEKeyboardHandler::INPUT_2] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
//}
//
//GLvoid AESprite::pressLeft() {
//	keyDown(AEKeyboardHandler::AKS_LEFT);
//	if (facing == FACING_RIGHT)
//		inputState[AEKeyboardHandler::INPUT_4] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
//	else
//		inputState[AEKeyboardHandler::INPUT_6] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
//}
//
//GLvoid AESprite::pressRight() {
//	keyDown(AEKeyboardHandler::AKS_RIGHT);
//	if (facing == FACING_RIGHT)
//		inputState[AEKeyboardHandler::INPUT_6] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
//	else
//		inputState[AEKeyboardHandler::INPUT_4] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
//}
//
//GLvoid AESprite::pressAttack() {
//	if (!isKeyDown(AEKeyboardHandler::BKS_ATTACK)) {
//		keyDown(AEKeyboardHandler::BKS_ATTACK);
//		if (isKeyDown(AEKeyboardHandler::AKS_UP)) {
//			inputState[AEKeyboardHandler::INPUT_8A] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
//		}
//		else if (isKeyDown(AEKeyboardHandler::AKS_DOWN)) {
//			inputState[AEKeyboardHandler::INPUT_2A] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
//		}
//		else if (isKeyDown(AEKeyboardHandler::AKS_RIGHT) && facing == FACING_RIGHT || isKeyDown(AEKeyboardHandler::AKS_LEFT) && facing == FACING_LEFT) {
//			inputState[AEKeyboardHandler::INPUT_6A] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
//		}
//		else if (isKeyDown(AEKeyboardHandler::AKS_RIGHT) && facing == FACING_LEFT || isKeyDown(AEKeyboardHandler::AKS_LEFT) && facing == FACING_RIGHT) {
//			inputState[AEKeyboardHandler::INPUT_4A] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
//		}
//		else {
//			inputState[AEKeyboardHandler::INPUT_A] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
//		}
//	}
//}
//
//GLvoid AESprite::pressJump() {
//	if (!isKeyDown(AEKeyboardHandler::BKS_JUMP)) {
//		keyDown(AEKeyboardHandler::BKS_JUMP);
//		inputState[AEKeyboardHandler::INPUT_J] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
//	}
//}
//
//GLvoid AESprite::pressDefend() {
//	if (!isKeyDown(AEKeyboardHandler::BKS_DEFEND)) {
//		keyDown(AEKeyboardHandler::BKS_DEFEND);
//		if (isKeyDown(AEKeyboardHandler::AKS_UP)) {
//			inputState[AEKeyboardHandler::INPUT_8D] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
//		}
//		else {
//			inputState[AEKeyboardHandler::INPUT_D] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
//		}
//	}
//}
//
//GLvoid AESprite::pressForward() {
//	if (facing == FACING_RIGHT)
//		pressRight();
//	else
//		pressLeft();
//}
//
//GLvoid AESprite::pressBackward() {
//	if (facing == FACING_LEFT)
//		pressRight();
//	else
//		pressLeft();
//}
//
//GLvoid AESprite::releaseUp() {
//	keyUp(AEKeyboardHandler::AKS_UP);
//}
//
//GLvoid AESprite::releaseDown() {
//	keyUp(AEKeyboardHandler::AKS_DOWN);
//}
//
//GLvoid AESprite::releaseLeft() {
//	keyUp(AEKeyboardHandler::AKS_LEFT);
//	if (facing == FACING_RIGHT && action == AEObject::STATE_CHAR_BACKWARD || facing == FACING_LEFT && action == AEObject::STATE_CHAR_FORWARD) {
//		changeAction(AEObject::ACT_CHAR_STAND);
//		setGroundSpeed(0.0);
//	}
//}
//
//GLvoid AESprite::releaseRight() {
//	keyUp(AEKeyboardHandler::AKS_RIGHT);
//	if (facing == FACING_RIGHT && action == AEObject::STATE_CHAR_FORWARD || facing == FACING_LEFT && action == AEObject::STATE_CHAR_BACKWARD) {
//		changeAction(AEObject::ACT_CHAR_STAND);
//		setGroundSpeed(0.0);
//	}	
//}
//
//GLvoid AESprite::releaseAttack() {
//	keyUp(AEKeyboardHandler::BKS_ATTACK);
//}
//
//GLvoid AESprite::releaseJump() {
//	keyUp(AEKeyboardHandler::BKS_JUMP);
//}
//
//GLvoid AESprite::releaseDefend() {
//	keyUp(AEKeyboardHandler::BKS_DEFEND);
//}
//
//GLvoid AESprite::input(INT _input) {
//	inputState[_input] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
//}

GLbyte AESprite::inputStateJudge(INT slot) {
	if (keyboardHandler != NULL && keyboardHandler->getInputState(slot) > 0)
		return 1;
	else
		return 0;
}

GLvoid AESprite::testKeyState() {
	//if (state >= AEObject::STATE_CHAR_ACTION)
	//	return;
	//if (inputStateJudge(AEKeyboardHandler::INPUT_A)) {
	//	changeAction(AEObject::ACT_CHAR_ATTACK_H);
	//}
	//else if (inputStateJudge(AEKeyboardHandler::INPUT_8A)) {
	//	changeAction(AEObject::ACT_CHAR_ATTACK_V);
	//}
	//else if (inputStateJudge(AEKeyboardHandler::INPUT_2A)) {
	//	changeAction(AEObject::ACT_CHAR_ATTACK);
	//}
	//else if (inputStateJudge(AEKeyboardHandler::INPUT_4A)) {
	//	changeAction(AEObject::ACT_CHAR_ATTACK_H);
	//}
	//else if (inputStateJudge(AEKeyboardHandler::INPUT_6A)) {
	//	changeAction(AEObject::ACT_CHAR_ATTACK_H);
	//}
	//else if (inputStateJudge(AEKeyboardHandler::INPUT_J)) {
	//	// Doing nothing
	//}
	//else if (inputStateJudge(AEKeyboardHandler::INPUT_D)) {
	//	changeAction(AEObject::ACT_CHAR_DEFEND_V);
	//}
	//else if (inputStateJudge(AEKeyboardHandler::INPUT_8D)) {
	//	changeAction(AEObject::ACT_CHAR_DEFEND_H);
	//}
	//else if (isKeyDown(AEKeyboardHandler::AKS_LEFT)) {
	//	if (action == AEObject::STATE_CHAR_STAND) {
	//		if (facing == FACING_RIGHT) {
	//			changeAction(AEObject::ACT_CHAR_BACKWARD);
	//			setGroundSpeed(-3.0);
	//		}
	//		else {
	//			changeAction(AEObject::ACT_CHAR_FORWARD);
	//			setGroundSpeed(3.0);
	//		}
	//	}
	//}
	//else if (isKeyDown(AEKeyboardHandler::AKS_RIGHT)) {
	//	if (action == AEObject::STATE_CHAR_STAND) {
	//		if (facing == FACING_RIGHT) {
	//			changeAction(AEObject::ACT_CHAR_FORWARD);
	//			setGroundSpeed(3.0);
	//		}
	//		else {
	//			changeAction(AEObject::ACT_CHAR_BACKWARD);
	//			setGroundSpeed(-3.0);
	//		}
	//	}
	//}
}

GLvoid AESprite::update() {
	AEAnimation* anim = (oid < 0 ? NULL : oTable.get(oid)->getAnim(action));
	if (oid >= 0) {
		if (timeToLive == 0) {
			changeAction(anim->getNext());
			return;
		}
		if (timeToLive > 0) timeToLive--;
	}
	INT fac = facing ? -1 : 1;
	cx += (fac * vx);
	cy += vy;
	vx += ax;
	vy += ay;
	angle += vangle;
	if (angle < -3.14159265f) angle += 2 * 3.14159265f;
	if (angle > 3.14159265f) angle -= 2 * 3.14159265f;
	time++;
	if (oid >= 0) {
		if (time >= anim->getEndTime(frame)) {
			cx += (fac * anim->getFrame(frame)->getShiftx());
			cy += anim->getFrame(frame)->getShifty();
			frame++;
			if (time >= anim->getEndTime(anim->getFrameCount() - 1)) {
				time = 0;
			}
			if (frame == anim->getFrameCount()) {
				frame = 0;
				if (!anim->isLoop()) {
					changeAction(anim->getNext());
					return;
				}
			}
		}
	}
}

//GLvoid AESprite::updateSample() {
//	AEBackground* bg = scene->getBackground();
//	for (int i = 0; i < AEKeyboardHandler::INPUT_COUNT; i++) {
//		if (inputState[i] > 0)
//			inputState[i]--;
//	}
//	if (timeToStiff > 0) {
//		timeToStiff--;
//		return;
//	}
//	AEAnimation* anim = oTable.get(oid)->getAnim(action);
//	if (timeToLive == 0) {
//		changeAction(anim->getNext());
//		return;
//	}
//	if (timeToLive > 0) timeToLive--;
//	INT fac;
//	if (facing) fac = -1; else fac = 1;
//	cx += (fac * vx);
//	cy += vy;
//	vx += ax;
//	vy += ay;
//	if (angle < -3.14159265f) angle += 2 * 3.14159265f;
//	if (angle > 3.14159265f) angle -= 2 * 3.14159265f;
//	if (state < 1000) {
//		if (state >= AEObject::STATE_CHAR_INAIR) {
//			if (drop + vy <= 0) {
//				drop = 0;
//				cy = bg->getLocation().y + bg->getLandform(onLandform).data[bg->getXonBG(INT(cx))].altitude;
//				if (state == AEObject::STATE_CHAR_INAIR) {
//					changeAction(AEObject::ACT_CHAR_LANDING);
//					vy = ay = 0.0;
//				}
//				else if (state == AEObject::STATE_CHAR_FALLING) {
//					if (abs(vy) >= AEPhysics::CHAR_REBOUND_SPEED) {
//						changeAction(AEObject::ACT_CHAR_FALL_REBOUND);
//						vy = 2.0;
//						ay = AEPhysics::GRAVITY;
//					}
//					else {
//						changeAction(AEObject::ACT_CHAR_LYING);
//						vy = ay = 0.0;
//					}
//				}
//				else if (state == AEObject::STATE_CHAR_FALLING_R) {
//					changeAction(AEObject::ACT_CHAR_LYING);
//					vy = ay = 0.0;
//				}
//			}
//			else {
//				onLandform = bg->getLandformIndexBelow(INT(cx), INT(cy), &drop);
//				if (state == AEObject::STATE_CHAR_FALLING) {
//					if (abs(vx) > 0.1) {
//						GLfloat slope = -vy / vx;
//						if (slope > tan(75 * 3.14159 / 180)) {
//							changeAction(AEObject::ACT_CHAR_FALL_90);
//						}
//						else if (slope > tan(45 * 3.14159 / 180)) {
//							changeAction(AEObject::ACT_CHAR_FALL_60);
//						}
//						else if (slope > tan(15 * 3.14159 / 180)) {
//							changeAction(AEObject::ACT_CHAR_FALL_30);
//						}
//						else if (slope > tan(-15 * 3.14159 / 180)) {
//							changeAction(AEObject::ACT_CHAR_FALL_0);
//						}
//						else {
//							changeAction(AEObject::ACT_CHAR_FALL_DROP);
//						}
//					}
//					else {
//						if (vy > 0.373) {
//							changeAction(AEObject::ACT_CHAR_FALL_90);
//						}
//						else if (vy > 0.100) {
//							changeAction(AEObject::ACT_CHAR_FALL_60);
//						}
//						else if (vy > 0.027) {
//							changeAction(AEObject::ACT_CHAR_FALL_30);
//						}
//						else if (vy > -0.027) {
//							changeAction(AEObject::ACT_CHAR_FALL_0);
//						}
//						else {
//							changeAction(AEObject::ACT_CHAR_FALL_DROP);
//						}
//					}
//					ay = AEPhysics::GRAVITY;
//				}
//			}
//		}
//		else {
//			INT lastLandform = onLandform;
//			onLandform = bg->getLandformIndexBelow(INT(cx), INT(cy), &drop);
//			if (abs(drop) <= AEBackground::ONLANDFORM_TOLERANCE) {
//				drop = 0;
//				cy = bg->getLocation().y + bg->getLandform(onLandform).data[bg->getXonBG(INT(cx))].altitude;
//				if (state >= AEObject::STATE_CHAR_ACTION) {
//					if (gndSpeed != 0.0) {
//						ax = AEPhysics::RESISTANCE_GROUND;
//						if (gndSpeed * ax > 0) ax = -ax;
//						GLfloat newSpeed = gndSpeed + ax;
//						if (newSpeed * gndSpeed <= 0) {
//							ax = newSpeed = 0.0;
//						}
//						gndSpeed = newSpeed;
//					}
//				}
//				GLfloat slope = bg->getLandform(onLandform).data[bg->getXonBG(INT(cx))].slope;
//				GLfloat string = sqrt(1.0 + slope * slope);
//				vx = gndSpeed * 1.0 / string;
//				vy = gndSpeed * fac * slope / string;
//			}
//			else {
//				changeAction(AEObject::ACT_CHAR_INAIR);
//				ay = AEPhysics::GRAVITY;
//			}
//		}
//	}
//	JumpParas* jump = anim->getFrame(frame).jumpTo;
//	KeyReleaseParas* release = anim->getFrame(frame).keyRelease;
//	if (jump != NULL) {
//		for (INT input = 0; input < AEKeyboardHandler::INPUT_COUNT; input++) {
//			if (jump->action[input] > 0 && inputStateJudge(input)) {
//				changeAction(jump->action[input]);
//				return;
//			}
//		}
//	}
//	if (release != NULL && (!isKeyDown(release->key))) {
//		changeAction(release->action);
//		return;
//	}
//	time++;
//	if (time >= anim->getEndTime(frame)) {
//		unlockAtkJudge();
//		cx += (fac * anim->getFrame(frame).shiftx);
//		cy += anim->getFrame(frame).shifty;
//		if (state <= AEObject::STATE_CHAR_INAIR) {
//			cy = bg->getLocation().y + bg->getLandform(onLandform).data[bg->getXonBG(INT(cx))].altitude;
//		}
//		angle += AEUtil::deg2rad(anim->getFrame(frame).rotate);
//		HoldParas* hold = anim->getFrame(frame).hold;
//		if (hold != NULL && isKeyDown(hold->key) > 0) {
//			if (frame == 0)
//				time = 0;
//			else
//				time = anim->getEndTime(frame - 1);
//		}	
//		else {
//			frame++;
//			if (time >= anim->getEndTime(anim->getFrameCount() - 1)) {
//				time = 0;
//			}
//			if (frame == anim->getFrameCount()) {
//				frame = 0;
//				if (!anim->isLoop()) {
//					changeAction(anim->getNext());
//					return;
//				}
//			}
//			if (state >= AEObject::STATE_CHAR_ACTION) {
//				if (state < AEObject::STATE_CHAR_INAIR) {
//					INT dvx = anim->getFrame(frame).dvx;
//					if (dvx == 999)
//						gndSpeed = 0;
//					else
//						gndSpeed += dvx;
//				}
//				else {
//					INT dvx = anim->getFrame(frame).dvx;
//					if (dvx == 999)
//						vx = 0;
//					else
//						vx += dvx;
//				}
//			}
//			if (anim->getFrame(frame).cast != NULL) {
//				Frame f = anim->getFrame(frame);
//				AEPoint castPoint = calcRotatedPoint(cx, cy, &f, angle, facing);
//				if (facing == 0)
//					scene->getSpriteTable()->add(new AESprite(scene, f.cast->oid, team, castPoint.x, castPoint.y, f.cast->action));
//				else
//					scene->getSpriteTable()->add(new AESprite(scene, f.cast->oid, team, castPoint.x, castPoint.y, f.cast->action, CAST_INVERSE));
//			}
//		}
//	}
//	testKeyState();
//}

GLvoid AESprite::paintShadow() {
	AEResource* shadow = rTable.get(101);
	AERect shadowRect = AEUtil::createRect(cx - shadow->getCellWidth() / 2, cy - drop - shadow->getCellHeight() / 2, cx + shadow->getCellWidth() / 2, cy - drop + shadow->getCellHeight() / 2);
	AEUtil::paintRect(shadow->getTexture(), AEUtil::createRect(0.0, 0.0, 1.0, 1.0), shadowRect);
}

GLvoid AESprite::paintCrosshair() {
	AEResource* crosshair = rTable.get(100);
	AERect crossRect = AEUtil::createRect(cx - crosshair->getCellWidth() / 2, cy - crosshair->getCellHeight() / 2, cx + crosshair->getCellWidth() / 2, cy + crosshair->getCellHeight() / 2);
	AEUtil::paintRect(crosshair->getTexture(), AEUtil::createRect(0.0, 0.0, 1.0, 1.0), crossRect);
}

GLvoid AESprite::paint() {
	glColor3f(1.0, 1.0, 1.0);
	AEObject* obj = oTable.get(oid);
	if (onLandform >= 0)
		paintShadow();
	AEFrame* f = obj->getAnim(action)->getFrame(frame);
	AEResource* res = f->getResource();
	AERect texClip = res->getTexCoords(f->getImgOffset(), f->getImgCells());
	AEBiasRect sprRect = calcRotatedRect(cx, cy, f, angle, facing);
	if (facing == FACING_RIGHT)
		AEUtil::paintRect(res->getTexture(), texClip, sprRect);
	else
		AEUtil::paintRect(res->getTexture(), AEUtil::getInversedRect(texClip, AEUtil::INVERSE_X), sprRect);
	//if (showCrosshair)
	//	paintCrosshair();
}

AESpriteTable::AESpriteTable() {
	maxIndex = -1;
	pHash = 0;
	for (int i = 0; i < MAX_ONLINE_SPRITES; i++) {
		occupied[i] = 0;
		hash[i] = -1;
	}
}

GLvoid AESpriteTable::add(AESprite* sp) {
	for (INT i = 0; i <= maxIndex; i++) {
		if (!occupied[i]) {
			sp->setIndex(i);
			table[i] = sp;  occupied[i] = 1;
			hash[pHash] = i;  pHash++;
			return;
		}
	}
	if (maxIndex == MAX_ONLINE_SPRITES - 1) {
		printf("Too many sprites online ..\n");
	}
	else {
		maxIndex++;
		sp->setIndex(maxIndex);
		table[maxIndex] = sp;  occupied[maxIndex] = 1;
		hash[pHash] = maxIndex;  pHash++;
	}
}

GLvoid AESpriteTable::addAt(INT index, AESprite* sp) {
	if (occupied[index]) {
		printf("Error on adding sprite: Slot Occupied.\n");
		return;
	}
	if (index > maxIndex) {
		maxIndex = index;
	}
	sp->setIndex(index);
	table[index] = sp;  occupied[index] = 1;
	hash[pHash] = maxIndex;  pHash++;
}

GLvoid AESpriteTable::remove(INT index) {
	if (!occupied[index]) {
		printf("Error on removing sprite: Slot Empty.\n");
		return;
	}
	delete table[index];
	table[index] = NULL;
	occupied[index] = 0;
	while (!occupied[maxIndex] && maxIndex > 0) maxIndex--;
	for (int i = 0; i < pHash; i++) {
		if (hash[i] == index) {
			pHash--;
			hash[i] = hash[pHash];
			hash[pHash] = -1;
			break;
		}
	}
}

GLvoid AESpriteTable::clear() {
	for (int i = 0; i <= maxIndex; i++) {
		if (occupied[i])
			remove(i);
	}
}

GLvoid AESpriteTable::handleCollisions() {

}

GLvoid AESpriteTable::update() {
	for (int i = 0; i <= maxIndex; i++) {
		if (occupied[i])
			table[i]->update();
	}
	handleCollisions();
}

GLvoid AESpriteTable::paint() {
	for (int i = 0; i <= maxIndex; i++) {
		if (occupied[i])
			table[i]->paint();
	}
}