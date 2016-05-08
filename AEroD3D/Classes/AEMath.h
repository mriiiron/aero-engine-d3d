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
#include <DirectXMath.h>
#include <cmath>
#include <string>

using namespace DirectX;


struct AEPointI {

    INT x, y;

    AEPointI() {
        x = y = 0;
    }

    AEPointI(INT _x, INT _y) {
        x = _x;  y = _y;
    }

};


struct AEPoint {

    FLOAT x, y;

    AEPoint() {
        x = y = 0.0f;
    }

    AEPoint(FLOAT _x, FLOAT _y) {
        x = _x;  y = _y;
    }

};


struct AERect {

    FLOAT x1, y1, x2, y2;

    AERect() {
        x1 = y1 = x2 = y2 = 0.0f;
    }

    AERect(FLOAT _x1, FLOAT _y1, FLOAT _x2, FLOAT _y2) {
        x1 = _x1;  y1 = _y1;  x2 = _x2;  y2 = _y2;
    }

};


struct AEBiasRect {

    FLOAT x1, y1, x2, y2, x3, y3, x4, y4;

    AEBiasRect() {
        x1 = y1 = x2 = y2 = x3 = y3 = x4 = y4 = 0.0f;
    }

    AEBiasRect(FLOAT _x1, FLOAT _y1, FLOAT _x2, FLOAT _y2, FLOAT _x3, FLOAT _y3, FLOAT _x4, FLOAT _y4) {
        x1 = _x1;  y1 = _y1;  x2 = _x2;  y2 = _y2; x3 = _x3;  y3 = _y3;  x4 = _x4;  y4 = _y4;
    }

};


struct SimpleVertex {

    XMFLOAT3 Pos;
    XMFLOAT2 Tex;
    XMFLOAT4 Clr;

    SimpleVertex() {
        Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
        Tex = XMFLOAT2(0.0f, 0.0f);
        Clr = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    SimpleVertex(XMFLOAT3 _Pos, XMFLOAT2 _Tex, XMFLOAT4 _Clr) {
        Pos = _Pos;
        Tex = _Tex;
        Clr = _Clr;
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
    XMFLOAT2 randomPointWithinCircle(FLOAT radius);

    FLOAT deg2rad(FLOAT deg);
    FLOAT rad2deg(FLOAT rad);
    
    FLOAT euclideanDistance(XMFLOAT2 p1, XMFLOAT2 p2);
    FLOAT euclideanDistance(AEPoint p1, AEPoint p2);

    BOOLEAN floatEqual(FLOAT f1, FLOAT f2, FLOAT threshold = 0.001f);
    BOOLEAN inBetween(FLOAT f, FLOAT lower, FLOAT upper);

    XMFLOAT2 rotatePoint(XMFLOAT2 p, XMFLOAT2 origin, FLOAT angle);

    INT log2(INT x);
    
    AERect flipRect(AERect rect, AEFlipDirection direction);
    AEBiasRect flipRect(AEBiasRect rect, AEFlipDirection direction);

    AERect moveRect(AERect rect, FLOAT dx, FLOAT dy);

    XMFLOAT2 lineIntersect(XMFLOAT2 A, XMFLOAT2 B, XMFLOAT2 C, XMFLOAT2 D);
    BOOLEAN pointEqual(XMFLOAT2 A, XMFLOAT2 B);
    BOOLEAN pointOnSegment(XMFLOAT2 P, XMFLOAT2 A, XMFLOAT2 B);

};

VOID paintRect(INT tex, AERect texClip, AERect paintArea);
VOID paintRect(INT tex, AERect texClip, AEBiasRect paintArea);
VOID paintRectByCenter(INT tex, AERect texClip, AEPoint center, FLOAT width, FLOAT height, FLOAT angleDeg);