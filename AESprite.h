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

#include "AEKeyboard.h"
#include "AEObject.h"

struct AERO_SPRITE_DESC {

	INT oid;
	INT team;
	FLOAT cx;
	FLOAT cy;
	INT action;
	INT inverse;

	AERO_SPRITE_DESC() {
		oid = team = 0;
		cx = cy = 0.0f;
		action = 0;
		inverse = AESprite::FACING_RIGHT;
	}

};


class AEScene;


class AESprite {

public:

	static const INT TEAM_NONE					= 0;
	static const INT TEAM_1						= 1;
	static const INT TEAM_2						= 2;
	static const INT TEAM_3						= 3;
	static const INT TEAM_4						= 4;
	static const INT CAST_INVERSE				= 1;
	static const INT FACING_RIGHT				= 0;
	static const INT FACING_LEFT				= 1;

	AESprite(AERO_SPRITE_DESC desc, AEScene* sceneRef);
	~AESprite() { delete keyboardHandler; }
	
	VOID setIndex(INT _index) { index = _index; }
	VOID setState(INT _state) { state = _state; }
	VOID setAx(FLOAT _ax) { ax = _ax; }
	VOID setAy(FLOAT _ay) { ay = _ay; }
	VOID setVx(FLOAT _vx) { vx = _vx; }
	VOID setVy(FLOAT _vy) { vy = _vy; }
	VOID setVAngle(FLOAT _vangle) { vangle = _vangle; }
	VOID setGroundSpeed(FLOAT _speed) { gndSpeed = _speed; }
	VOID setAngle(FLOAT _angle) { angle = _angle; }
	VOID rotateDeg(FLOAT degree) { angle += AEUtil::deg2rad(degree); }
	VOID rotateRad(FLOAT rad) { angle += rad; }
	VOID setFacing(INT _facing) { facing = _facing; }
	VOID turnOver() { facing = 1 - facing; }
	VOID resetKeys() { keyState = 0; }
	VOID setHPValue(INT _hpValue) { hpValue = _hpValue; }
	VOID stiffen(INT _time) { timeToStiff = _time; }
	VOID lockAtkJudge() { atkJudgeLock = 1; }
	VOID unlockAtkJudge() { atkJudgeLock = 0; }
	VOID keyDown(INT _key) { keyState = keyState | _key; }
	VOID keyUp(INT _key) { keyState = keyState & ~_key; }
	BYTE getFacing() { return facing; }
	BOOLEAN isAtkJudgeLocked() { return atkJudgeLock; }
	BOOLEAN isKeyDown(INT _key) { return _key & keyState; }
	INT getIndex() { return index; }
	INT getOid() { return oid; }
	INT getAction() { return action; }
	INT getFrame() { return frame; }
	INT getState() { return state; }
	INT getTeam() { return team; }
	INT getStiffTime() { return timeToStiff; }
	INT getGroundSpeed() { return gndSpeed; }
	FLOAT getCx() { return cx; }
	FLOAT getCy() { return cy; }
	AEPoint getCenter() { return AEUtil::createPoint(cx, cy); }
	FLOAT getAngle() { return angle; }
	FLOAT getVx() { return vx; }
	FLOAT getVy() { return vy; }
	FLOAT getVAngle() { return vangle; }
	FLOAT getAx() { return ax; }
	FLOAT getAy() { return ay; }
	AEVector2 getFaceVector();
	BOOLEAN inputStateJudge(INT _input);
	INT log2(INT key);
	AEPoint calcRotatedPoint(AEPoint point, FLOAT cx, FLOAT cy, AEFrame* f, FLOAT angle, GLbyte facing);
	AEBiasRect calcRotatedRect(FLOAT cx, FLOAT cy, AEFrame* f, FLOAT angle, GLbyte facing);
	/* CHARACTER ONLY */
	INT getKeyState() { return keyState; }
	INT getHP() { return hpValue; }
	AEScene* getScene() { return scene; }
	AEKeyboardHandler* getKeyboardHandler() { return keyboardHandler; }
	VOID takeDamage(INT damage) { hpValue -= damage; }
	//VOID pressUp();
	//VOID pressDown();
	//VOID pressLeft();
	//VOID pressRight();
	//VOID pressAttack();
	//VOID pressJump();
	//VOID pressDefend();
	//VOID pressForward();
	//VOID pressBackward();
	//VOID releaseUp();
	//VOID releaseDown();
	//VOID releaseLeft();
	//VOID releaseRight();
	//VOID releaseAttack();
	//VOID releaseJump();
	//VOID releaseDefend();
	//VOID input(INT _input);
	string getObjName();
	VOID changeAction(INT _action);
	VOID toNextFrame(AEAnimation anim);
	VOID paintShadow();
	VOID paintCrosshair();
	VOID paint();

	virtual VOID testKeyState();
	virtual VOID update();

protected:

	INT index, oid, action, frame, time, timeToLive, timeToStiff;
	FLOAT cx, cy, vx, vy, ax, ay, angle, vangle, gndSpeed;
	INT state, team, keyState, drop, onLandform;
	BYTE facing, atkJudgeLock;
	INT hpValue, hpMax;
	
	AEScene* sceneRef;
	AEKeyboardHandler* keyboardHandler;

};


class AESpriteTable {

public:

	static const INT MAX_ONLINE_SPRITES		= 120;

	AESpriteTable();
	AESprite* get(INT index) { return table[index]; }
	AESprite* getByHash(INT hashIndex) { return table[hash[hashIndex]]; }
	INT getHashCount() { return pHash; }
	VOID add(AESprite* sp);
	VOID addAt(INT index, AESprite* sp);
	VOID remove(INT index);
	VOID clear();
	VOID handleCollisions();
	VOID update();
	VOID paint();

private:

	AESprite* table[MAX_ONLINE_SPRITES];
	INT occupied[MAX_ONLINE_SPRITES];
	INT hash[MAX_ONLINE_SPRITES];
	INT maxIndex, pHash;

};