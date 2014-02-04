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

struct AEPoint {

	FLOAT x, y;

	AEPoint(FLOAT _x, FLOAT _y) {
		_x = x;  _y = y;
	}

};

struct AERect {

	FLOAT x1, y1, x2, y2;

	AERect(FLOAT _x1, FLOAT _y1, FLOAT _x2, FLOAT _y2) {
		x1 = _x1;  y1 = _y1;  x2 = _x2;  y2 = _y2;
	}

};


struct AEBiasRect {

	FLOAT x1, y1, x2, y2, x3, y3, x4, y4;

	AEBiasRect(FLOAT _x1, FLOAT _y1, FLOAT _x2, FLOAT _y2, FLOAT _x3, FLOAT _y3, FLOAT _x4, FLOAT _y4) {
		x1 = _x1;  y1 = _y1;  x2 = _x2;  y2 = _y2; x3 = _x3;  y3 = _y3;  x4 = _x4;  y4 = _y4;
	}

};

class AEVector2 {

public:

	AEVector2(FLOAT _x, FLOAT _y) { x = _x;  y = _y; }

	static AEVector2 normalize(AEVector2 v);
	static FLOAT crossProduct(AEVector2 v1, AEVector2 v2);
	static FLOAT dotProduct(AEVector2 v1, AEVector2 v2);
	static FLOAT angleBetween(AEVector2 v1, AEVector2 v2);
	static AEVector2 rad2vec(FLOAT angleRad);
	static FLOAT vec2rad(AEVector2 v);

	FLOAT model() { return sqrtf(x * x + y * y); }

	FLOAT x, y;

};

namespace AENSMath {

	const FLOAT PI = 3.14159265f;

	enum AEFlipDirection {
		HORIZONTAL, VERTICAL
	};

	INT randomIntBetween(INT start, INT end);
	
	FLOAT randomClampf();

	FLOAT deg2rad(FLOAT deg);
	FLOAT rad2deg(FLOAT rad);
	
	FLOAT euclideanDistance(AEPoint p1, AEPoint p2);
	
	AERect flipRect(AERect rect, AEFlipDirection direction);
	AEBiasRect flipRect(AEBiasRect rect, AEFlipDirection direction);

};

VOID paintRect(INT tex, AERect texClip, AERect paintArea);
VOID paintRect(INT tex, AERect texClip, AEBiasRect paintArea);
VOID paintRectByCenter(INT tex, AERect texClip, AEPoint center, FLOAT width, FLOAT height, FLOAT angleDeg);