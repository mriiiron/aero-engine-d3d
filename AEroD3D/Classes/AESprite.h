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

#include "AETable.h"
#include "AEPlatform.h"
#include "AECollision.h"
#include "AEObject.h"
#include "AEAI.h"

#include "XTK\SpriteBatch.h"


class AEScene;


struct AERO_SPRITE_DESC {

	AEObject* obj;
	INT team;
	FLOAT cx;
	FLOAT cy;
	FLOAT layerDepth;
	INT action;
	SpriteEffects flip;

	AERO_SPRITE_DESC() {
		obj = nullptr;
		team = 0;
		cx = cy = 0.0f;
		layerDepth = 0.0f;
		action = 0;
		flip = SpriteEffects_None;
	}

};


class AESprite {

public:

	static const INT CAST_INVERSE = 1;
	static const INT FACING_RIGHT = 0;
	static const INT FACING_LEFT = 1;

	static const INT ACTION_NUM_DEAD = 1000;

	static const INT TIME_TO_LIVE_UNLIMITED = -1;

	static const INT ANGLE_BOTH = 0;
	static const INT ANGLE_DIRECTION = 1;
	static const INT ANGLE_DISPLAY = 2;

	AESprite(AERO_SPRITE_DESC desc);

	// Get'n'set
	VOID setIndex(INT _index) { index = _index; }
	VOID setState(INT _state) { state = _state; }
	VOID setCx(FLOAT _cx) { cx = _cx; }
	VOID setCy(FLOAT _cy) { cy = _cy; }
	VOID setAx(FLOAT _ax) { ax = _ax; }
	VOID setAy(FLOAT _ay) { ay = _ay; }
	VOID setVx(FLOAT _vx) { vx = _vx; }
	VOID setVy(FLOAT _vy) { vy = _vy; }
	VOID setGroundSpeed(FLOAT _speed) { gndSpeed = _speed; }
	VOID setFlip(SpriteEffects _flip) { flip = _flip; }
	VOID setHPValue(INT _hpValue) { hpValue = _hpValue; }
	VOID setAlpha(FLOAT _alpha) { alpha = _alpha; }
	VOID setLayerDepth(FLOAT _layerDepth) { layerDepth = _layerDepth; }
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
	INT getFlip() { return flip; }
	FLOAT getGroundSpeed() { return gndSpeed; }
	FLOAT getCx() { return cx; }
	FLOAT getCy() { return cy; }
	FLOAT getVx() { return vx; }
	FLOAT getVy() { return vy; }
	FLOAT getSpeedModelSquare() { return vx * vx + vy * vy; }
	FLOAT getSpeedModel() { return sqrtf(getSpeedModelSquare()); }
	FLOAT getAx() { return ax; }
	FLOAT getAy() { return ay; }
	FLOAT getAlpha() { return alpha; }
	FLOAT getLayerDepth() { return layerDepth; }
	AEPoint getCenter() { return AEPoint(cx, cy); }
	AEObject* getObject() { return obj; }
	AEAnimation* getCurrentAnimation() { return obj->getAnim(action); }
	AEFrame* getCurrentFrame() { return obj->getAnim(action)->getFrame(frameNum); }
	std::string getObjName() { return obj->getName(); }
	AEAI* getAI() { return ai; }
	AEScene* getScene() { return scene; }

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

	VOID turnOverHorizontally() { if (flip == SpriteEffects_None) flip = SpriteEffects_FlipHorizontally; else if (flip == SpriteEffects_FlipHorizontally) flip = SpriteEffects_None; }

	FLOAT getAngle(INT option = ANGLE_DIRECTION);
	FLOAT getVAngle(INT option = ANGLE_DIRECTION);
	VOID setAngleRad(FLOAT rad, INT option = ANGLE_BOTH);
	VOID setAngleDeg(FLOAT degree, INT option = ANGLE_BOTH);
	VOID setVAngleRad(FLOAT _vAngle, INT option = ANGLE_BOTH);
	VOID setVAngleDeg(FLOAT _vAngle, INT option = ANGLE_BOTH);
	VOID rotateRad(FLOAT rad, INT option = ANGLE_BOTH);
	VOID rotateDeg(FLOAT degree, INT option = ANGLE_BOTH);

	VOID createAttachmentTable(INT size);
	BOOLEAN hasAttachments() { return (attachmentTable == nullptr ? FALSE : TRUE); }
	AEHashedTable<AESprite>* getAttachmentTable() { return attachmentTable; }

	VOID changeAction(INT _action);
	VOID toNextFrame(AEAnimation anim);

	/* THESE FUNCTIONS ARE TOTALLY USELESS */
	AEPoint calcRotatedPoint(AEPoint point, FLOAT cx, FLOAT cy, AEFrame* f, FLOAT angle, BYTE flip);
	AERect calcSpriteRect(FLOAT cx, FLOAT cy, AEFrame* f, BYTE flip);
	RECT calcSpriteRectInRECT(FLOAT cx, FLOAT cy, AEFrame* f, BYTE flip);
	XMFLOAT2 calcSpriteDrawingPosition(FLOAT cx, FLOAT cy, AEFrame* f, BYTE flip);
	AEBiasRect calcRotatedSpriteRect(FLOAT cx, FLOAT cy, AEFrame* f, FLOAT angle, BYTE flip);
	/* I'M NOT KIDDING */

	XMVECTOR getFacingVector(INT option = ANGLE_DIRECTION);
	XMVECTOR getVelocityVector();

	VOID platformCollisionCheck(FLOAT cx_old, FLOAT cy_old, AEHashedTable<AEPlatform>* platformTable);

	virtual VOID applyControl();
	virtual VOID platformCollision(AEPlatform* platform, INT tailNodeIndex, AECollisionResult collisionResult);
	virtual VOID update(AEHashedTable<AEPlatform>* platformTable = nullptr);
	virtual VOID render();

protected:

	AEObject* obj;
	AEScene* scene = nullptr;
	AEAI* ai = nullptr;
	AEHashedTable<AESprite>* attachmentTable = nullptr;

	INT action, team;
	FLOAT cx, cy;
	SpriteEffects flip;
	BOOLEAN deadFlag;
	INT timeToLive;

	INT index = 0, frameNum = 0, time = 0, timeToStiff = 0;
	FLOAT vx = 0.0f, vy = 0.0f, ax = 0.0f, ay = 0.0f, alpha = 1.0f, angle = 0.0f, angleDisplay = 0.0f, vAngle = 0.0f, vAngleDisplay = 0.0f, gndSpeed = 0.0f;
	FLOAT layerDepth = 0.0f;
	INT state = 0, keyState = 0, drop, onLandform = 0;
	BOOLEAN atkJudgeLock = FALSE;
	INT hpValue = 100, hpMax = 100;
	INT attachmentTableSize = 0;

	VOID updateAttachments(FLOAT hostdx, FLOAT hostdy);

};