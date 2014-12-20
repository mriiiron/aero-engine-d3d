/**
 * @file	AECamera.h
 * @brief	Defining the control of ae_Camera.
 *
 * @author	CaiyZ (Mr.IroN)
 * @version	0.1
 * @date	2013
*/

#pragma once

#include "AESprite.h"


class AECamera {

public:

	const static INT MODE_NONE = 0;
	const static INT MODE_STABLE = 1;
	const static INT MODE_CHASE = 2;
	const static INT MODE_KEYBOARD = 3;

	AECamera();

	VOID setFocus(FLOAT _x, FLOAT _y) { focus.x = _x;  focus.y = _y; }
	VOID setVx(FLOAT _vx) { vx = _vx; }
	VOID setVy(FLOAT _vy) { vy = _vy; }
	VOID setMode(INT _mode) { mode = _mode; }
	VOID setTarget(AESprite* _target) { target = _target; }
	VOID moveFocus(FLOAT dx, FLOAT dy) { focus.x += dx;  focus.y += dy; }
	XMFLOAT2 getFocus() { return focus; }
	XMFLOAT2 getAdjustedFocus() { return adjustedFocus; }

	VOID move(FLOAT dx, FLOAT dy) { focus.x += dx;  focus.y += dy; }
	VOID update();

	VOID shake(INT time, INT amplitude);

private:

	INT mode;
	XMFLOAT2 focus, adjustedFocus;
	FLOAT vx, vy;
	AESprite* target;
	INT shakeTimer, shakeAmp;

};