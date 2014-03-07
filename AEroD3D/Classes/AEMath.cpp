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

FLOAT AENSMath::deg2rad(FLOAT deg) {
	return deg / 180.0f * 3.14159f;
}

FLOAT AENSMath::rad2deg(FLOAT rad) {
	return rad / 3.14159f * 180.0f;
}

FLOAT AENSMath::euclideanDistance(AEPoint p1, AEPoint p2) {
	return sqrtf((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
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