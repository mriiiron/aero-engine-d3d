#include <d3d11_1.h>
#include <string>
#include <cmath>
#include "AEMath.h"
#include "AEPhysics.h"
#include "AETable.h"
#include "AEResource.h"
#include "AEBackground.h"
#include "AESprite.h"

extern AEConstantTable<AEObject>				objectTable;

AESprite::AESprite(AERO_SPRITE_DESC desc) {
	state = 0;
	index = 0;
	obj = desc.obj;  action = desc.action;  team = desc.team;  cx = desc.cx;  cy = desc.cy;
	vx = vy = ax = ay = angle = vangle = gndSpeed = 0.0f;
	frameNum = time = facing = timeToStiff = keyState = atkJudgeLock = 0;
	onLandform = 0;
	hpValue = hpMax = 100;
	facing = desc.facing;
	if (action > 0) {
		changeAction(desc.action);
	}
	deadFlag = FALSE;
}

AEPoint AESprite::getCenter() {
	return AEPoint(cx, cy);
}

std::string AESprite::getObjName() {
	return obj->getName();
}

AEPoint AESprite::calcRotatedPoint(AEPoint point, FLOAT cx, FLOAT cy, AEFrame* f, FLOAT angle, BYTE facing) {
	FLOAT cosA = cos(angle), sinA = sin(angle);
	FLOAT x, y;
	if (facing == FACING_RIGHT) {
		x = cx + (point.x - f->getCenterx()) * cosA - (point.y - f->getCentery()) * sinA;
		y = cy + (point.x - f->getCenterx()) * sinA + (point.y - f->getCentery()) * cosA;
	}
	else {
		x = cx - (point.x - f->getCenterx()) * cosA + (point.y - f->getCentery()) * sinA;
		y = cy + (point.x - f->getCenterx()) * sinA + (point.y - f->getCentery()) * cosA;
	}
	return AEPoint(x, y);
}

AERect AESprite::calcRect(FLOAT cx, FLOAT cy, AEFrame* f, BYTE facing) {
	INT centerx = f->getCenterx(), centery = f->getCentery();
	INT width = f->getWidth(), height = f->getHeight();
	FLOAT x1, y1, x2, y2;
	if (facing == FACING_RIGHT) {
		x1 = cx - centerx;
		y1 = cy - centery;
		x2 = x1 + width;
		y2 = y1 + height;
	}
	else {
		x2 = cx + centerx;
		y1 = cy - centery;
		x1 = x2 - width;
		y2 = y1 + height;
	}
	return AERect(x1, y1, x2, y2);
}

AEBiasRect AESprite::calcRotatedRect(FLOAT cx, FLOAT cy, AEFrame* f, FLOAT angle, BYTE facing) {
	FLOAT cosA = cos(angle), sinA = sin(angle);
	INT centerx = f->getCenterx(), centery = f->getCentery();
	INT width = f->getWidth(), height = f->getHeight();
	FLOAT x1, y1, x2, y2, x3, y3, x4, y4;
	if (facing == FACING_RIGHT) {
		x1 = cx - centerx * cosA + centery * sinA;
		y1 = cy - centerx * sinA - centery * cosA;
		x2 = x1 + width * cosA;
		y2 = y1 + width * sinA;
		x3 = x2 - height * sinA;
		y3 = y2 + height * cosA;
		x4 = x1 - height * sinA;
		y4 = y1 + height * cosA;
	}
	else {
		x2 = cx + centerx * cosA - centery * sinA;
		y2 = cy - centerx * sinA - centery * cosA;
		x1 = x2 - width * cosA;
		y1 = y2 + width * sinA;
		x3 = x2 + height * sinA;
		y3 = y2 + height * cosA;
		x4 = x1 + height * sinA;
		y4 = y1 + height * cosA;
	}
	return AEBiasRect(x1, y1, x2, y2, x3, y3, x4, y4);
}

VOID AESprite::changeAction(INT _action) {
	if (_action == 1000) {
		deadFlag = TRUE;
		return;
	}
	action = _action;
	AEAnimation* anim = obj->getAnim(action);
	state = anim->getState();
	timeToLive = anim->getTTL();
	frameNum = time = 0;
	INT dvx = anim->getFrame(frameNum)->getDvx();
	if (dvx == 999)
		vx = 0;
	else
		vx += dvx;
	INT dvy = anim->getFrame(frameNum)->getDvy();
	if (dvy == 999)
		vy = 0;
	else
		vy += dvy;
}

VOID AESprite::applyControl() {

}

VOID AESprite::update() {
	AEAnimation* anim = obj->getAnim(action);
	if (timeToLive == 0) {
		changeAction(anim->getNext());
		return;
	}
	if (timeToLive > 0) timeToLive--;
	INT fac = facing ? -1 : 1;
	cx += (fac * vx);
	cy += vy;
	vx += ax;
	vy += ay;
	angle += vangle;
	if (angle < -AENSMath::PI) angle += 2 * AENSMath::PI;
	if (angle > AENSMath::PI) angle -= 2 * AENSMath::PI;
	time++;
	if (time >= anim->getEndTime(frameNum)) {
		cx += (fac * anim->getFrame(frameNum)->getShiftx());
		cy += anim->getFrame(frameNum)->getShifty();
		frameNum++;
		if (time >= anim->getEndTime(anim->getFrameCount() - 1)) {
			time = 0;
		}
		if (frameNum == anim->getFrameCount()) {
			frameNum = 0;
			if (!anim->isLoop()) {
				changeAction(anim->getNext());
				return;
			}
		}
	}
}

VOID AESprite::addToRenderBuffer(FLOAT zValue) {
	AEFrame* f = obj->getAnim(action)->getFrame(frameNum);
	AEResource* res = f->getResource();
	AERect texClip = res->getTexClip(f->getImgOffset(), f->getImgCells(), facing);
	if (angle == 0.0f) {
		AERect paintArea = calcRect(cx, cy, f, facing);
		res->addToRenderBuffer(paintArea, texClip, zValue);
	}
	else {
		AEBiasRect paintArea = calcRotatedRect(cx, cy, f, angle, facing);
		res->addToRenderBuffer(paintArea, texClip, zValue);
	}
}