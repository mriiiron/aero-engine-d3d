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
	FLOAT scale;
	INT action;
	SpriteEffects flip;

	AERO_SPRITE_DESC() {
		obj = nullptr;
		team = 0;
		cx = cy = 0.0f;
		layerDepth = 0.0f;
		scale = 1.0f;
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

	static const INT RENDER_OPTION_NORMAL = 0;
	static const INT RENDER_OPTION_SCALE = 1;
	static const INT RENDER_OPTION_WIPE = 2;
	static const INT RENDER_OPTION_OFFSET = 3;
	static const INT RENDER_OPTION_TILT = 4;

	static const INT RENDER_WIPE_LEFT = 0;
	static const INT RENDER_WIPE_RIGHT = 1;
	static const INT RENDER_WIPE_TOP = 2;
	static const INT RENDER_WIPE_BOTTOM = 3;

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
	VOID setFlip(SpriteEffects _flip) { flip = _flip; }
	VOID setHPValue(INT _hpValue) { hpValue = _hpValue; }
	VOID setAlpha(FLOAT _alpha) { alpha = _alpha; }
	VOID setScale(FLOAT _scale) { scale = _scale; }
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
	UINT getKeyState() { return keyState; }
	INT getHP() { return hpValue; }
	INT getHPMax() { return hpMax; }
	FLOAT getHPProportion() { return 1.0f * hpValue / hpMax; }
	INT getFlip() { return flip; }
	FLOAT getCx() { return cx; }
	FLOAT getCy() { return cy; }
	FLOAT getVx() { return vx; }
	FLOAT getVy() { return vy; }
	FLOAT getSpeedModelSquare() { return vx * vx + vy * vy; }
	FLOAT getSpeedModel() { return sqrtf(getSpeedModelSquare()); }
	FLOAT getAx() { return ax; }
	FLOAT getAy() { return ay; }
	FLOAT getAlpha() { return alpha; }
	FLOAT getScale() { return scale; }
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

	VOID keyDown(UINT _key) { keyState |= _key; }
	VOID keyUp(UINT _key) { keyState &= ~_key; }
	BOOLEAN isKeyPressed(UINT _key) { return ((_key & keyState) == 0 ? false : true); }

	VOID takeDamage(INT damage) { hpValue -= damage; }
	VOID remove() { deadFlag = true; }
	BOOLEAN isAtkJudgeLocked() { return atkJudgeLock; }
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
	AERect calcSpriteRect(FLOAT cx, FLOAT cy, AEFrame* f, SpriteEffects flip);
	RECT calcSpriteRectInRECT(FLOAT cx, FLOAT cy, AEFrame* f, SpriteEffects flip);
	XMFLOAT2 calcSpriteDrawingPosition(FLOAT cx, FLOAT cy, AEFrame* f, BYTE flip);
	AEBiasRect calcRotatedSpriteRect(FLOAT cx, FLOAT cy, AEFrame* f, FLOAT angle, BYTE flip);
	/* I'M NOT KIDDING */

	XMVECTOR getFacingVector(INT option = ANGLE_DIRECTION);
	XMVECTOR getVelocityVector();

	VOID platformCollisionCheck(FLOAT cx_old, FLOAT cy_old, AEHashedTable<AEPlatform>* platformTable);

	VOID updateAttachments();

	virtual VOID applyControl();
	virtual VOID platformCollision(AEPlatform* platform, INT tailNodeIndex, AECollisionResult collisionResult);
	virtual VOID update(AEHashedTable<AEPlatform>* platformTable = nullptr);
	virtual VOID render(INT renderOption = RENDER_OPTION_NORMAL, ...);

protected:

	AEObject* obj;
	AEScene* scene;
	AEAI* ai;
	AEHashedTable<AESprite>* attachmentTable;

	INT action, team;
	FLOAT cx, cy;
	SpriteEffects flip;
	BOOLEAN deadFlag;
	INT timeToLive;

	INT index, frameNum, time, timeToStiff;
	FLOAT vx, vy, ax, ay, angle, angleDisplay, vAngle, vAngleDisplay;
	FLOAT alpha, layerDepth, scale;
	INT state;
	UINT keyState;
	BOOLEAN atkJudgeLock = FALSE;
	INT hpValue, hpMax;
	INT attachmentTableSize = 0;

};