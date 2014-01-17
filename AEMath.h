/**
 * @file	AEMath.h
 * @brief	Containing items that are commonly used in other files.
 *
 * @author	CaiyZ (Mr.IroN)
 * @version	0.1
 * @date	2013
*/

#pragma once

#include <d3d11_1.h>
#include <cmath>
#include <string>

#define NULL								0

typedef struct AEPoint {
	FLOAT x, y;
};

typedef struct AERect {
	FLOAT x1, y1, x2, y2;
};


typedef struct AEBiasRect {
	FLOAT x1, y1, x2, y2, x3, y3, x4, y4;
};

class AEVector2 {

public:

	AEVector2(FLOAT _x, FLOAT _y) { x = _x;  y = _y; }

	static AEVector2 normalize(AEVector2 v);
	static FLOAT cross(AEVector2 v1, AEVector2 v2);
	static FLOAT dotProduct(AEVector2 v1, AEVector2 v2);
	static FLOAT angleBetween(AEVector2 v1, AEVector2 v2);

	FLOAT getModel() { return sqrtf(x * x + y * y); }

	FLOAT x, y;

};


class AEMath {

public:

	static FLOAT deg2rad(FLOAT deg) { return deg / 180.0f * 3.14159f; }
	static FLOAT rad2deg(FLOAT rad) { return rad / 3.14159f * 180.0f; }

	static FLOAT calcDistance(AEPoint p1, AEPoint p2) { return sqrtf((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)); }
	static INT randomIntBetween(INT start, INT end) { return (rand() % (end - start + 1)) + start; }
	static FLOAT randomClampf() { return rand() / double(RAND_MAX); }
	static AEPoint createPoint(FLOAT _x, FLOAT _y) { AEPoint p;  p.x = _x;  p.y = _y;  return p; }
	static AERect createRect(FLOAT _x1, FLOAT _y1, FLOAT _x2, FLOAT _y2) { AERect r;  r.x1 = _x1;  r.y1 = _y1;  r.x2 = _x2;  r.y2 = _y2;  return r; }
	
	static AERect getInversedRect(AERect r, INT direction);
	static AEBiasRect getInversedRect(AEBiasRect r, INT direction);
	static VOID paintRect(INT tex, AERect texClip, AERect paintArea);
	static VOID paintRect(INT tex, AERect texClip, AEBiasRect paintArea);
	static VOID paintRectByCenter(INT tex, AERect texClip, AEPoint center, FLOAT width, FLOAT height, FLOAT angleDeg);

};