/**
 * @file	AEResource.h
 * @brief	Defining the resources being used.
 *
 * @author	CaiyZ (Mr.IroN)
 * @version	0.1
 * @date	2013
*/

#pragma once

#include "AEMath.h"


typedef enum AEResType {
	RES_UNKNOWN, RES_1x1, RES_1x5, RES_2x5, RES_4x5, RES_5x10, RES_FONT
};


class AEResource {

public:

	AEResource(AERO_RESOURCE_DESC desc);
	INT getTexture() { return tex; }
	INT getCellWidth() { return cellW; }
	INT getCellHeight() { return cellH; }
	AERect getTexCoords(INT imgOffset, INT imgCells);

private:
	
	INT tex;
	AEResType rtype;
	INT cellW, cellH;

};


struct AERO_RESOURCE_DESC {

	INT tex;
	INT cellW, cellH;
	AEResType rtype;

	AERO_RESOURCE_DESC() {
		tex = 0;
		cellW = cellH = 0;
		rtype = RES_UNKNOWN;
	}

};