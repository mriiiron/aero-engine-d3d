#include "AEMath.h"

AEVector2 AEVector2::rad2vec(FLOAT angleRad) {
	if (angleRad == AENSMath::PI / 2) {
		return AEVector2(0.0f, 1.0f);
	}
	else if (angleRad == -AENSMath::PI / 2) {
		return AEVector2(0.0f, -1.0f);
	}
	else if (angleRad > -AENSMath::PI / 2 && angleRad < AENSMath::PI / 2) {
		return AEVector2(1.0f, tan(angleRad));
	}
	else {
		return AEVector2(-1.0f, -tan(angleRad));
	}
}

FLOAT AEVector2::vec2rad(AEVector2 v) {
	if (v.x == 0.0f) {
		if (v.y > 0.0f) {
			return AENSMath::PI / 2.0f;
		}
		else if (v.y < 0.0f) {
			return -AENSMath::PI / 2.0f;
		}
		else {
			return 0.0f;
		}
	}
	else {
		return atan(v.y / v.x);
	}
}


INT AENSMath::randomIntBetween(INT start, INT end) {
	return (rand() % (end - start + 1)) + start;
}
	
FLOAT AENSMath::randomClampf() {
	return rand() / FLOAT(RAND_MAX);
}

XMFLOAT2 AENSMath::randomPointWithinCircle(FLOAT radius) {
	FLOAT dist = randomClampf() * radius;
	FLOAT angle = randomClampf() * 2.0f * PI;
	return XMFLOAT2(dist * cosf(angle), dist * sinf(angle));
}

FLOAT AENSMath::deg2rad(FLOAT deg) {
	return deg / 180.0f * 3.14159f;
}

FLOAT AENSMath::rad2deg(FLOAT rad) {
	return rad / 3.14159f * 180.0f;
}

FLOAT AENSMath::euclideanDistance(XMFLOAT2 p1, XMFLOAT2 p2) {
	return sqrtf((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

BOOLEAN AENSMath::floatEqual(FLOAT f1, FLOAT f2, FLOAT threshold) {
	return (fabs(f1 - f2) < threshold) ? TRUE : FALSE;
}

XMFLOAT2 AENSMath::rotatePoint(XMFLOAT2 p, XMFLOAT2 origin, FLOAT angle) {
	FLOAT cosA = cosf(angle), sinA = sinf(angle);
	FLOAT dx = p.x - origin.x, dy = p.y - origin.y;
	FLOAT x = origin.x + dx * cosA - dy * sinA;
	FLOAT y = origin.y + dx * sinA + dy * cosA;
	return XMFLOAT2(x, y);
}

INT AENSMath::log2(INT x) {
	switch (x) {
	case 0x1: return 0;
	case 0x2: return 1;
	case 0x4: return 2;
	case 0x8: return 3;
	case 0x10: return 4;
	case 0x20: return 5;
	case 0x40: return 6;
	default: return -1;
	}
}

AERect AENSMath::flipRect(AERect rect, AEFlipDirection direction) {
	if (direction == HORIZONTAL) {
		return AERect(rect.x2, rect.y1, rect.x1, rect.y2);
	}
	else {
		return AERect(rect.x1, rect.y2, rect.x2, rect.y1);
	}
}

AEBiasRect AENSMath::flipRect(AEBiasRect rect, AEFlipDirection direction) {
	if (direction == HORIZONTAL) {
		return AEBiasRect(rect.x2, rect.y2, rect.x1, rect.y1, rect.x4, rect.y4, rect.x3, rect.y3);
	}
	else {
		return AEBiasRect(rect.x4, rect.y4, rect.x3, rect.y3, rect.x2, rect.y2, rect.x1, rect.y1);
	}
}

// Assuming AB and CD is not collineate or parallel
XMFLOAT2 AENSMath::lineIntersect(XMFLOAT2 A, XMFLOAT2 B, XMFLOAT2 C, XMFLOAT2 D) {
	XMFLOAT2 result;
	if (B.x == A.x) {
		result = { A.x, (D.y - C.y) / (D.x - C.x) * (A.x - C.x) + C.y };
	}
	else if (C.x == D.x) {
		result = { C.x, (B.y - A.y) / (B.x - A.x) * (C.x - A.x) + A.y };
	}
	else {
		FLOAT a = (B.y - A.y) / (B.x - A.x), b = -1.0f, e = a * A.x - A.y;
		FLOAT c = (D.y - C.y) / (D.x - C.x), d = -1.0f, f = c * C.x - C.y;
		FLOAT denominator = a * d - b * c;
		result = { (e * d - b * f) / denominator, (a * f - e * c) / denominator };
	}
	return result;
}

BOOLEAN AENSMath::pointEqual(XMFLOAT2 A, XMFLOAT2 B) {
	return A.x == B.x && A.y == B.y;
}

BOOLEAN AENSMath::pointOnSegment(XMFLOAT2 P, XMFLOAT2 A, XMFLOAT2 B) {
	if ((A.x < B.x ? A.x : B.x) <= P.x && P.x <= (A.x > B.x ? A.x : B.x) && (A.y < B.y ? A.y : B.y) <= P.y && P.y <= (A.y > B.y ? A.y : B.y)) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}