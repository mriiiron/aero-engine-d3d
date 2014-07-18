#include <d3d11_1.h>
#include <DirectXMath.h>

#include "AECollision.h"

AECollisionResult AENSCollision::rectAndRect(AERect r1, AERect r2) {
	AECollisionResult result;
	if (r1.x1 <= r2.x2 && r1.y1 <= r2.y2 && r1.x2 >= r2.x1 && r1.y2 >= r2.y1) {
		result.isCollided = TRUE;
		result.point = { 0.0f, 0.0f }; // TODO
	}
	else {
		result.isCollided = FALSE;
	}
	return result;
}

// Segment 1 as AB; Segment 2 as CD
AECollisionResult AENSCollision::vectorAndSegment(XMFLOAT2 A, XMFLOAT2 B, XMFLOAT2 C, XMFLOAT2 D){
	AECollisionResult result;

	// AB or CD cannot be just a point
	if (AENSMath::pointEqual(A, B) || AENSMath::pointEqual(C, D)) {
		result.isCollided = FALSE;
		return result;
	}

	FLOAT r1left = (A.x < B.x ? A.x : B.x), r1right = A.x + B.x - r1left;
	FLOAT r1top = (A.y < B.y ? A.y : B.y), r1bottom = A.y + B.y - r1top;
	FLOAT r2left = (C.x < D.x ? C.x : D.x), r2right = C.x + D.x - r2left;
	FLOAT r2top = (C.y < D.y ? C.y : D.y), r2bottom = C.y + D.y - r2top;

	if (AENSCollision::rectAndRect(AERect(r1left, r1right, r1top, r1bottom), AERect(r2left, r2right, r2top, r2bottom)).isCollided) {

		// Vector AB with segment CD
		XMFLOAT2 vecABf = { B.x - A.x, B.y - A.y };
		XMFLOAT2 vecACf = { C.x - A.x, C.y - A.y };
		XMFLOAT2 vecADF = { D.x - A.x, D.y - A.y };
		XMFLOAT2 vecCDf = { D.x - C.x, D.y - C.y };
		XMFLOAT2 vecCAf = { A.x - C.x, A.y - C.y };
		XMFLOAT2 vecCBF = { B.x - C.x, B.y - C.y };
		XMVECTOR vecAB = XMLoadFloat2(&vecABf);
		XMVECTOR vecAC = XMLoadFloat2(&vecACf);
		XMVECTOR vecAD = XMLoadFloat2(&vecADF);
		XMVECTOR vecCD = XMLoadFloat2(&vecCDf);
		XMVECTOR vecCA = XMLoadFloat2(&vecCAf);
		XMVECTOR vecCB = XMLoadFloat2(&vecCBF);

		// If A is on line CD
		if (AENSMath::floatEqual(XMVectorGetZ(XMVector2Cross(vecCA, vecCD)), 0.0f)) {

			// If A is on segment CD, including nodes
			if (AENSMath::pointEqual(A, C) || AENSMath::pointEqual(A, D) || AENSMath::pointOnSegment(A, C, D)) {

				// If B is also on line CD
				if (AENSMath::floatEqual(XMVectorGetZ(XMVector2Cross(vecCD, vecCB)), 0.0f)) {
					result.isCollided = FALSE;
				}

				// If B is not on line CD
				else {

					// If B is on left side of CD
					if (XMVectorGetZ(XMVector2Cross(vecCD, vecCB)) < 0.0f) {
						result.isCollided = FALSE;
					}

					// If B is on right side of CD
					else {
						result.isCollided = TRUE;
						result.point = A;
					}

				}

			}

			// If A is out of segment CD
			else {
				result.isCollided = FALSE;
			}
		
		}

		// If A is on right side of CD, there is no way to collide with CD
		else if (XMVectorGetZ(XMVector2Cross(vecCD, vecCA)) > 0.0f) {
			result.isCollided = FALSE;
			return result;
		}

		// If A is on left side of CD
		else {

			// If B is on line CD
			if (AENSMath::floatEqual(XMVectorGetZ(XMVector2Cross(vecCB, vecCD)), 0.0f)) {

				// If B is on segment CD, including nodes
				if (AENSMath::pointEqual(B, C) || AENSMath::pointEqual(B, D) || AENSMath::pointOnSegment(B, C, D)) {

					// If A is on left side of CD
					if (XMVectorGetZ(XMVector2Cross(vecCD, vecCA)) < 0.0f) {
						result.isCollided = TRUE;
						result.point = B;
					}

					// If A is on right side of CD
					else {
						result.isCollided = FALSE;
					}

				}

				// If B is out of segment CD
				else {
					result.isCollided = FALSE;
				}

			}

			// If B is also not on line CD (both A and B are not on CD, which is most common)
			else {
				FLOAT crossProduct_AB_AC = XMVectorGetZ(XMVector2Cross(vecAB, vecAC));
				FLOAT crossProduct_AB_AD = XMVectorGetZ(XMVector2Cross(vecAB, vecAD));
				if (crossProduct_AB_AC * crossProduct_AB_AD > 0.0f) {
					result.isCollided = FALSE;
				}
				else {
					FLOAT crossProduct_CD_CA = XMVectorGetZ(XMVector2Cross(vecCD, vecCA));
					FLOAT crossProduct_CD_CB = XMVectorGetZ(XMVector2Cross(vecCD, vecCB));
					if (crossProduct_CD_CA * crossProduct_CD_CB > 0.0f) {
						result.isCollided = FALSE;
					}
					else {
						result.isCollided = TRUE;
						result.point = AENSMath::lineIntersect(A, B, C, D);
					}
				}
			}

		}

	}
	else {
		result.isCollided = FALSE;
	}
	return result;
}