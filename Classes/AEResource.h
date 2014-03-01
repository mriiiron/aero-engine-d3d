/**
 * @file	AEResource.h
 * @brief	Defining the resources being used.
 *
 * @author	CaiyZ (Mr.IroN)
 * @version	0.1
 * @date	2013
*/

#pragma once

#include <vector>
#include "AEMath.h"


typedef enum AEResType {
	RES_UNKNOWN, RES_1x1, RES_1x5, RES_2x5, RES_4x5, RES_5x10, RES_FONT
};


struct AERO_RESOURCE_DESC {

	INT rid;
	ID3D11ShaderResourceView* tex;
	INT cellW, cellH;
	AEResType rtype;

	AERO_RESOURCE_DESC() {
		rid = 0;
		tex = nullptr;
		cellW = cellH = 0;
		rtype = RES_UNKNOWN;
	}

};


class AEResource {

public:

	AEResource(AERO_RESOURCE_DESC desc);

	ID3D11ShaderResourceView* getTexture() { return tex; }
	INT getCellWidth() { return cellW; }
	INT getCellHeight() { return cellH; }

	AERect getTexClip(INT imgOffset, INT imgCellCount);
	VOID addToRenderBuffer(AERect paintArea, AERect texClip);
	VOID addToRenderBuffer(AEBiasRect paintArea, AERect texClip);
	VOID clearRenderBuffer(); 
	VOID render();

private:
	
	INT rid;
	ID3D11ShaderResourceView* tex;
	AEResType rtype;
	INT cellW, cellH;

	std::vector<SimpleVertex> vertexBuffer;
	// std::vector<INT> indexBuffer;

};