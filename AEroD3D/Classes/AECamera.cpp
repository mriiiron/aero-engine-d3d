#include <d3d11_1.h>
#include "AEMath.h"
#include "AECamera.h"

AECamera::AECamera() {
	mode = CAMMODE_NONE;
	focus = AEPoint(0.0f, 0.0f);
	vx = vy = 0.0f;
	targetID = 0;
}