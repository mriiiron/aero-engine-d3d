#include <d3d11_1.h>
#include <algorithm>
#include <string>
#include "AEResource.h"

extern ID3D11DeviceContext*					g_pImmediateContext;
extern ID3D11Buffer*						g_pVertexBuffer;
extern ID3D11Buffer*						g_pIndexBuffer;

AEResource::AEResource(AERO_RESOURCE_DESC desc) {
	rid = desc.rid;
	tex = desc.tex;
	rtype = desc.rtype;
	cellW = desc.cellW;
	cellH = desc.cellH;
}

INT AEResource::getCellCount() {
	switch (rtype) {
	case RES_1x1:
		return 1;
	case RES_1x5:
		return 5;
	case RES_1x10:
		return 10;
	case RES_2x5:
		return 10;
	case RES_4x5:
		return 20;
	case RES_5x10:
		return 50;
	default:
		return 0;
	}
}

AERect AEResource::getTexClip(INT imgOffset, INT imgCellCount, BYTE inverse) {
	FLOAT x1, x2, y1, y2;
	switch (rtype) {
	case RES_1x1:
		x1 = 0.0f;
		x2 = 1.0f;
		y1 = 0.0f;
		y2 = 1.0f;
		break;
	case RES_1x5:
		x1 = 0.2f * (imgOffset % 5);
		x2 = x1 + 0.2f * imgCellCount;
		y1 = 0.0f;
		y2 = 1.0f;
		break;
	case RES_1x10:
		x1 = 0.1f * (imgOffset % 10);
		x2 = x1 + 0.1f * imgCellCount;
		y1 = 0.0f;
		y2 = 1.0f;
		break;
	case RES_2x5:
		x1 = 0.2f * (imgOffset % 5);
		x2 = x1 + 0.2f * imgCellCount;
		y1 = 0.5f * (imgOffset / 5);
		y2 = y1 + 0.5f;
		break;
	case RES_4x5:
		x1 = 0.2f * (imgOffset % 5);
		x2 = x1 + 0.2f * imgCellCount;
		y1 = 0.25f * (imgOffset / 5);
		y2 = y1 + 0.25f;
		break;
	case RES_5x10:
		x1 = 0.1f * (imgOffset % 10);
		x2 = x1 + 0.1f * imgCellCount;
		y1 = 0.2f * (imgOffset / 10);
		y2 = y1 + 0.2f;
		break;
	default:
		// Error
		break;
	}
	switch (inverse) {
	case TEXCLIP_INVERSE_X:
		std::swap(x1, x2);
		break;
	case TEXCLIP_INVERSE_Y:
		std::swap(y1, y2);
		break;
	case TEXCLIP_INVERSE_BOTH:
		std::swap(x1, x2);
		std::swap(y1, y2);
		break;
	default:
		break;
	}
	return AERect(x1, y1, x2, y2);
}

RECT AEResource::getTexClipInTexel(INT imgOffset, INT imgCellCount, BYTE inverse) {
	UINT x1, x2, y1, y2;
	switch (rtype) {
	case RES_1x1:
		x1 = 0;
		x2 = cellW;
		y1 = 0;
		y2 = cellH;
		break;
	case RES_1x5:
		x1 = cellW * (imgOffset % 5);
		x2 = x1 + cellW * imgCellCount;
		y1 = 0;
		y2 = cellH;
		break;
	case RES_1x10:
		x1 = cellW * (imgOffset % 10);
		x2 = x1 + cellW * imgCellCount;
		y1 = 0;
		y2 = cellH;
		break;
	case RES_2x5:
		x1 = cellW * (imgOffset % 5);
		x2 = x1 + cellW * imgCellCount;
		y1 = cellH * (imgOffset / 5);
		y2 = y1 + cellH;
		break;
	case RES_4x5:
		x1 = cellW * (imgOffset % 5);
		x2 = x1 + cellW * imgCellCount;
		y1 = cellH * (imgOffset / 5);
		y2 = y1 + cellH;
		break;
	case RES_5x10:
		x1 = cellW * (imgOffset % 10);
		x2 = x1 + cellW * imgCellCount;
		y1 = cellH * (imgOffset / 10);
		y2 = y1 + cellH;
		break;
	default:
		// Error
		break;
	}
	switch (inverse) {
	case TEXCLIP_INVERSE_X:
		std::swap(x1, x2);
		break;
	case TEXCLIP_INVERSE_Y:
		std::swap(y1, y2);
		break;
	case TEXCLIP_INVERSE_BOTH:
		std::swap(x1, x2);
		std::swap(y1, y2);
		break;
	default:
		break;
	}
	return RECT{ x1, y1, x2, y2 };
}

VOID AEResource::addToRenderBuffer(AERect paintArea, AERect texClip, FLOAT alpha, FLOAT zValue) {
	XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, alpha);
	vertexBuffer.push_back(SimpleVertex(XMFLOAT3(paintArea.x1, paintArea.y2, zValue), XMFLOAT2(texClip.x1, texClip.y1), color));
	vertexBuffer.push_back(SimpleVertex(XMFLOAT3(paintArea.x2, paintArea.y2, zValue), XMFLOAT2(texClip.x2, texClip.y1), color));
	vertexBuffer.push_back(SimpleVertex(XMFLOAT3(paintArea.x1, paintArea.y1, zValue), XMFLOAT2(texClip.x1, texClip.y2), color));
	vertexBuffer.push_back(SimpleVertex(XMFLOAT3(paintArea.x1, paintArea.y1, zValue), XMFLOAT2(texClip.x1, texClip.y2), color));
	vertexBuffer.push_back(SimpleVertex(XMFLOAT3(paintArea.x2, paintArea.y2, zValue), XMFLOAT2(texClip.x2, texClip.y1), color));
	vertexBuffer.push_back(SimpleVertex(XMFLOAT3(paintArea.x2, paintArea.y1, zValue), XMFLOAT2(texClip.x2, texClip.y2), color));
}

VOID AEResource::addToRenderBuffer(AEBiasRect paintArea, AERect texClip, FLOAT alpha, FLOAT zValue) {
	XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, alpha);
	vertexBuffer.push_back(SimpleVertex(XMFLOAT3(paintArea.x4, paintArea.y4, zValue), XMFLOAT2(texClip.x1, texClip.y1), color));
	vertexBuffer.push_back(SimpleVertex(XMFLOAT3(paintArea.x3, paintArea.y3, zValue), XMFLOAT2(texClip.x2, texClip.y1), color));
	vertexBuffer.push_back(SimpleVertex(XMFLOAT3(paintArea.x1, paintArea.y1, zValue), XMFLOAT2(texClip.x1, texClip.y2), color));
	vertexBuffer.push_back(SimpleVertex(XMFLOAT3(paintArea.x1, paintArea.y1, zValue), XMFLOAT2(texClip.x1, texClip.y2), color));
	vertexBuffer.push_back(SimpleVertex(XMFLOAT3(paintArea.x3, paintArea.y3, zValue), XMFLOAT2(texClip.x2, texClip.y1), color));
	vertexBuffer.push_back(SimpleVertex(XMFLOAT3(paintArea.x2, paintArea.y2, zValue), XMFLOAT2(texClip.x2, texClip.y2), color));
}

VOID AEResource::clearRenderBuffer() {
	vertexBuffer.clear();
	// indexBuffer.clear();
}

VOID AEResource::render() {
	D3D11_MAPPED_SUBRESOURCE mapped;
	g_pImmediateContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, &vertexBuffer[0], sizeof(SimpleVertex) * vertexBuffer.size());
	g_pImmediateContext->Unmap(g_pVertexBuffer, 0);
	g_pImmediateContext->PSSetShaderResources(0, 1, &tex);
	g_pImmediateContext->Draw(vertexBuffer.size(), 0);
}

VOID AEResource::releaseTexture() {
	if (tex) tex->Release();
}


AEResourceTable::AEResourceTable(INT _maxElemCount) : AEConstantTable<AEResource>(_maxElemCount) {
	
}

VOID AEResourceTable::renderAndClear() {
	for (int i = 0; i < maxElemCount; i++) {
		if (occupied[i]) {
			AEResource* res = table[i];
			if (!res->isBufferEmpty()) {
				res->render();
				res->clearRenderBuffer();
			}
		}
	}
}