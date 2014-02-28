/**
 * @file	AECamera.h
 * @brief	Defining the control of camera.
 *
 * @author	CaiyZ (Mr.IroN)
 * @version	0.1
 * @date	2013
*/

#pragma once


enum AECameraMode {
	CAMMODE_NONE, CAMMODE_STABLE, CAMMODE_CHASE, CAMMODE_KEYBOARD
};


class AECamera {

public:

	AECamera();

	VOID move();
	VOID update();
	VOID changeTarget();
	VOID setFocus(FLOAT _x, FLOAT _y) { focus.x = _x;  focus.y = _y; }
	VOID setVx(FLOAT _vx) { vx = _vx; }
	VOID setVy(FLOAT _vy) { vy = _vy; }
	VOID setMode(AECameraMode _mode) { mode = _mode; }
	VOID setTargetID(INT _targetID) { targetID = _targetID; }
	VOID moveFocus(FLOAT dx, FLOAT dy) { focus.x += dx;  focus.y += dy; }
	AEPoint getFocus() { return focus; }

private:

	AECameraMode mode;
	AEPoint focus;
	FLOAT vx, vy;
	INT targetID;

};