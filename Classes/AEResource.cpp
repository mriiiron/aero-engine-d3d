#include <d3d11_1.h>
#include "AEResource.h"

extern ID3D11DeviceContext*					g_pImmediateContext = nullptr;
extern ID3D11Buffer*						g_pVertexBuffer = nullptr;
extern ID3D11Buffer*						g_pIndexBuffer = nullptr;

AEResource::AEResource(AERO_RESOURCE_DESC desc) {
	tex = desc.tex;
	rtype = desc.rtype;
	cellW = desc.cellW;
	cellH = desc.cellH;
}

AERect AEResource::getTexClip(INT imgOffset, INT imgCellCount) {
	FLOAT x1, x2, y1, y2;
	switch (rtype) {
	case RES_1x1:
		x1 = 0.0;
		x2 = 1.0;
		y2 = 1.0;
		y1 = 0.0;
		break;
	case RES_1x5:
		x1 = 0.2 * (imgOffset % 5);
		x2 = x1 + 0.2 * imgCellCount;
		y2 = 1.0;
		y1 = 0.0;
		break;
	case RES_2x5:
		x1 = 0.2 * (imgOffset % 5);
		x2 = x1 + 0.2 * imgCellCount;
		y2 = 1.0 - 0.5 * (imgOffset / 5);
		y1 = y2 - 0.5;
		break;
	case RES_4x5:
		x1 = 0.2 * (imgOffset % 5);
		x2 = x1 + 0.2 * imgCellCount;
		y2 = 1.0 - 0.25 * (imgOffset / 5);
		y1 = y2 - 0.25;
		break;
	case RES_5x10:
		x1 = 0.1 * (imgOffset % 10);
		x2 = x1 + 0.1 * imgCellCount;
		y2 = 1.0 - 0.2 * (imgOffset / 10);
		y1 = y2 - 0.2;
		break;
	default:
		break;
	}
	return AERect(x1, y1, x2, y2);
}

VOID AEResource::addToRenderBuffer(AERect paintArea, AERect texClip) {
	vertexBuffer.push_back(SimpleVertex(XMFLOAT3(paintArea.x1, paintArea.y1, 0.0f), XMFLOAT2(texClip.x1, texClip.y1)));
	vertexBuffer.push_back(SimpleVertex(XMFLOAT3(paintArea.x1, paintArea.y2, 0.0f), XMFLOAT2(texClip.x1, texClip.y2)));
	vertexBuffer.push_back(SimpleVertex(XMFLOAT3(paintArea.x2, paintArea.y1, 0.0f), XMFLOAT2(texClip.x2, texClip.y1)));
	vertexBuffer.push_back(SimpleVertex(XMFLOAT3(paintArea.x2, paintArea.y1, 0.0f), XMFLOAT2(texClip.x2, texClip.y1)));
	vertexBuffer.push_back(SimpleVertex(XMFLOAT3(paintArea.x1, paintArea.y2, 0.0f), XMFLOAT2(texClip.x1, texClip.y2)));
	vertexBuffer.push_back(SimpleVertex(XMFLOAT3(paintArea.x2, paintArea.y2, 0.0f), XMFLOAT2(texClip.x2, texClip.y2)));
}

VOID AEResource::addToRenderBuffer(AEBiasRect paintArea, AERect texClip) {
	vertexBuffer.push_back(SimpleVertex(XMFLOAT3(paintArea.x1, paintArea.y1, 0.0f), XMFLOAT2(texClip.x1, texClip.y1)));
	vertexBuffer.push_back(SimpleVertex(XMFLOAT3(paintArea.x4, paintArea.y4, 0.0f), XMFLOAT2(texClip.x1, texClip.y2)));
	vertexBuffer.push_back(SimpleVertex(XMFLOAT3(paintArea.x2, paintArea.y2, 0.0f), XMFLOAT2(texClip.x2, texClip.y1)));
	vertexBuffer.push_back(SimpleVertex(XMFLOAT3(paintArea.x2, paintArea.y2, 0.0f), XMFLOAT2(texClip.x2, texClip.y1)));
	vertexBuffer.push_back(SimpleVertex(XMFLOAT3(paintArea.x4, paintArea.y4, 0.0f), XMFLOAT2(texClip.x1, texClip.y2)));
	vertexBuffer.push_back(SimpleVertex(XMFLOAT3(paintArea.x3, paintArea.y3, 0.0f), XMFLOAT2(texClip.x2, texClip.y2)));
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