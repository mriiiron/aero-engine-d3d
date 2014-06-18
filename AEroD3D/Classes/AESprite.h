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
#include "AEAI.h"

#include "XTK\SpriteBatch.h"


class AEScene;


struct AERO_SPRITE_DESC {

	AEObject* obj;
	INT team;
	FLOAT cx;
	FLOAT cy;
	INT action;
	SpriteEffects facing;

	AERO_SPRITE_DESC() {
		obj = nullptr;
		team = 0;
		cx = cy = 0.0f;
		action = 0;
		facing = SpriteEffects_None;
	}

};


class AESprite {

public:

	static const INT CAST_INVERSE = 1;
	static const INT FACING_RIGHT = 0;
	static const INT FACING_LEFT = 1;

	static const INT ACTION_NUM_DEAD = 1000;

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
	VOID setFacing(SpriteEffects _facing) { facing = _facing; }
	VOID setHPValue(INT _hpValue) { hpValue = _hpValue; }
	VOID setAlpha(FLOAT _alpha) { alpha = _alpha; }
	VOID adjustAlpha(FLOAT dAlpha) { alpha += dAlpha;  if (alpha < 0.0f) alpha = 0.0f;  if (alpha > 1.0f) alpha = 1.0f; }
	VOID move(FLOAT dx, FLOAT dy) { cx += dx;  cy += dy; }
	VOID setAI(AEAI* _ai) { ai = _ai; }
	VOID setScene(AEScene* _scene) { scene = _scene; }
	INT getIndex() { return index; }
	INT getAction() { return action; }
	INT getFrameNum() { return frameNum; }
	INT getState() { return state; }
	INT getTeam() { return team; }
	INT getStiffTime() { return timeToStiff; }
	INT getKeyState() { return keyState; }
	INT getHP() { return hpValue; }
	SpriteEffects getFacing() { return facing; }
	FLOAT getGroundSpeed() { return gndSpeed; }
	FLOAT getCx() { return cx; }
	FLOAT getCy() { return cy; }
	FLOAT getAngle() { return angle; }
	FLOAT getVx() { return vx; }
	FLOAT getVy() { return vy; }
	FLOAT getVAngle() { return vangle; }
	FLOAT getAx() { return ax; }
	FLOAT getAy() { return ay; }
	FLOAT getAlpha() { return alpha; }
	AEPoint getCenter() { return AEPoint(cx, cy); }
	AEObject* getObject() { return obj; }
	std::string getObjName() { return obj->getName(); }
	AEScene* getScene() { return scene; }

	VOID rotateDeg(FLOAT degree) { angle += AENSMath::deg2rad(degree); }
	VOID rotateRad(FLOAT rad) { angle += rad; }
	VOID turnOver() { facing = (facing == SpriteEffects_None ? SpriteEffects_FlipHorizontally : SpriteEffects_None); }
	VOID resetKeyState() { keyState = 0; }
	VOID stiffen(INT _time) { timeToStiff = _time; }
	VOID lockAtkJudge() { atkJudgeLock = 1; }
	VOID unlockAtkJudge() { atkJudgeLock = 0; }
	VOID keyDown(INT _key) { keyState = keyState | _key; }
	VOID keyUp(INT _key) { keyState = keyState & ~_key; }
	VOID takeDamage(INT damage) { hpValue -= damage; }
	VOID kill() { deadFlag = TRUE; }
	BOOLEAN isAtkJudgeLocked() { return atkJudgeLock; }
	BOOLEAN isKeyDown(INT _key) { return (BOOLEAN)(_key & keyState); }
	BOOLEAN isDead() { return deadFlag; }

	VOID changeAction(INT _action);
	VOID toNextFrame(AEAnimation anim);

	/* THESE FUNCTIONS ARE TOTALLY USELESS */
	AEPoint calcRotatedPoint(AEPoint point, FLOAT cx, FLOAT cy, AEFrame* f, FLOAT angle, BYTE facing);
	AERect calcSpriteRect(FLOAT cx, FLOAT cy, AEFrame* f, BYTE facing);
	RECT calcSpriteRectInRECT(FLOAT cx, FLOAT cy, AEFrame* f, BYTE facing);
	XMFLOAT2 calcSpriteDrawingPosition(FLOAT cx, FLOAT cy, AEFrame* f, BYTE facing);
	AEBiasRect calcRotatedSpriteRect(FLOAT cx, FLOAT cy, AEFrame* f, FLOAT angle, BYTE facing);
	/* I'M NOT KIDDING */

	virtual VOID applyControl();
	virtual VOID update();
	virtual VOID render(FLOAT zValue = 0.0f);

protected:

	AEObject* obj;
	AEScene* scene;
	AEAI* ai;
	//TODO: AEHashedTable<AESprite*> attatchments;

	INT index, action, frameNum, time, timeToLive, timeToStiff;
	FLOAT cx, cy, vx, vy, ax, ay, alpha, angle, vangle, gndSpeed;
	INT state, team, keyState, drop, onLandform;
	BOOLEAN atkJudgeLock;
	SpriteEffects facing;
	INT hpValue, hpMax;
	BOOLEAN deadFlag;

};