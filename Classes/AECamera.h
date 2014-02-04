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
	VOID setFocus(FLOAT _x, FLOAT _y) { crosshair.x = _x;  crosshair.y = _y; }
	VOID setVx(FLOAT _vx) { vx = _vx; }
	VOID setVy(FLOAT _vy) { vy = _vy; }
	VOID setMode(AECameraMode _mode) { mode = _mode; }
	VOID setTarget(INT _target) { target = _target; }
	VOID shiftCrossHair(FLOAT _shiftx, FLOAT _shifty) { crosshair.x += _shiftx;  crosshair.y += _shifty; }
	AEPoint getCrosshair() { return crosshair; }

private:

	INT mode;
	AEPoint crosshair;
	FLOAT vx, vy;
	INT target;

};