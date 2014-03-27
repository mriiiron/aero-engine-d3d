/**
 * @file	AESprite.h
 * @brief	Defining all "dynamic objects" in the scenes.
 *
 * Sprites could be dynamically created and destroyed while the game is running.
 * A certain scene helds a sprite table to manage its sprites.
 *
 * @author	CaiyZ (Mr.IroN)
 * @version	0.1
 * @date	2013
*/

#pragma once

#include "AEObject.h"


class AEScene;


struct AERO_SPRITE_DESC {

	AEObject* obj;
	INT team;
	FLOAT cx;
	FLOAT cy;
	INT action;
	INT facing;

	AERO_SPRITE_DESC() {
		obj = nullptr;
		team = 0;
		cx = cy = 0.0f;
		action = 0;
		facing = 0;
	}

};


class AESprite {

public:

	static const INT CAST_INVERSE				= 1;
	static const INT FACING_RIGHT				= 0;
	static const INT FACING_LEFT				= 1;

	AESprite(AERO_SPRITE_DESC desc);
	
	// Get'n'set
	VOID setIndex(INT _index) { index = _index; }
	VOID setState(INT _state) { state = _state; }
	VOID setAx(FLOAT _ax) { ax = _ax; }
	VOID setAy(FLOAT _ay) { ay = _ay; }
	VOID setVx(FLOAT _vx) { vx = _vx; }
	VOID setVy(FLOAT _vy) { vy = _vy; }
	VOID setVAngle(FLOAT _vangle) { vangle = _vangle; }
	VOID setGroundSpeed(FLOAT _speed) { gndSpeed = _speed; }
	VOID setAngle(FLOAT _angle) { angle = _angle; }
	VOID setFacing(BYTE _facing) { facing = _facing; }
	VOID setHPValue(INT _hpValue) { hpValue = _hpValue; }
	INT getIndex() { return index; }
	INT getAction() { return action; }
	INT getFrameNum() { return frameNum; }
	INT getState() { return state; }
	INT getTeam() { return team; }
	INT getStiffTime() { return timeToStiff; }
	INT getKeyState() { return keyState; }
	INT getHP() { return hpValue; }
	BYTE getFacing() { return facing; }
	FLOAT getGroundSpeed() { return gndSpeed; }
	FLOAT getCx() { return cx; }
	FLOAT getCy() { return cy; }
	FLOAT getAngle() { return angle; }
	FLOAT getVx() { return vx; }
	FLOAT getVy() { return vy; }
	FLOAT getVAngle() { return vangle; }
	FLOAT getAx() { return ax; }
	FLOAT getAy() { return ay; }
	AEPoint getCenter();
	AEObject* getObject() { return obj; }
	std::string getObjName();

	VOID rotateDeg(FLOAT degree) { angle += AENSMath::deg2rad(degree); }
	VOID rotateRad(FLOAT rad) { angle += rad; }
	VOID turnOver() { facing = 1 - facing; }
	VOID resetKeyState() { keyState = 0; }
	VOID stiffen(INT _time) { timeToStiff = _time; }
	VOID lockAtkJudge() { atkJudgeLock = 1; }
	VOID unlockAtkJudge() { atkJudgeLock = 0; }
	VOID keyDown(INT _key) { keyState = keyState | _key; }
	VOID keyUp(INT _key) { keyState = keyState & ~_key; }
	VOID takeDamage(INT damage) { hpValue -= damage; }
	BOOLEAN isAtkJudgeLocked() { return atkJudgeLock; }
	BOOLEAN isKeyDown(INT _key) { return (BOOLEAN)(_key & keyState); }

	VOID changeAction(INT _action);
	VOID toNextFrame(AEAnimation anim);

	AEPoint calcRotatedPoint(AEPoint point, FLOAT cx, FLOAT cy, AEFrame* f, FLOAT angle, BYTE facing);
	AERect calcRect(FLOAT cx, FLOAT cy, AEFrame* f, BYTE facing);
	AEBiasRect calcRotatedRect(FLOAT cx, FLOAT cy, AEFrame* f, FLOAT angle, BYTE facing);

	virtual VOID applyControl();
	virtual VOID update();
	virtual VOID addToRenderBuffer();

protected:

	AEObject* obj;
	INT index, action, frameNum, time, timeToLive, timeToStiff;
	FLOAT cx, cy, vx, vy, ax, ay, angle, vangle, gndSpeed;
	INT state, team, keyState, drop, onLandform;
	BYTE facing, atkJudgeLock;
	INT hpValue, hpMax;
	BOOLEAN deadFlag;

};