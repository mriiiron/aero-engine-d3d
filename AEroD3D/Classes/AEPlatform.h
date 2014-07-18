#pragma once

#include <DirectXMath.h>

using namespace DirectX;


struct AEPlatformSegment {

};


class AEPlatform {

public:

	AEPlatform(INT _nodeCount);
	~AEPlatform();

	INT getNodeCount() { return nodeCount; }
	INT getSegmentCount() { return nodeCount - 1; }
	XMFLOAT2 getNode(INT index);
	VOID addNode(XMFLOAT2 node);

private:

	INT maxIndex = 0, nodeCount;
	XMFLOAT2* nodeTable;

};