#pragma once

#include "AEMath.h"

using namespace DirectX;


struct AECollisionResult {
	
	BOOLEAN isCollided;
	XMFLOAT2 point;

	AECollisionResult() {
		isCollided = FALSE;
		point = { 0.0f, 0.0f };
	}

};


namespace AENSCollision {

	AECollisionResult rectAndRect(AERect r1, AERect r2);
	AECollisionResult rectAndRect(RECT r1, RECT r2);
	AECollisionResult vectorAndSegment(XMFLOAT2 A, XMFLOAT2 B, XMFLOAT2 C, XMFLOAT2 D);

}