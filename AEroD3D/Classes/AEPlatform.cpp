#include <d3d11_1.h>

#include "AETable.h"
#include "AEPlatform.h"

AEPlatform::AEPlatform(INT _nodeCount) {
	maxIndex = 0;
	nodeCount = _nodeCount;
	nodeTable = new XMFLOAT2[nodeCount];
}

AEPlatform::~AEPlatform() {
	delete [] nodeTable;
}

XMFLOAT2 AEPlatform::getNode(INT index) {
	if (index < 0) {
		return nodeTable[0];
	}
	else if (index >= nodeCount) {
		return nodeTable[nodeCount - 1];
	}
	else {
		return nodeTable[index];
	}
}

VOID AEPlatform::addNode(XMFLOAT2 node) {
	if (maxIndex >= nodeCount) {
		return;
	}
	nodeTable[maxIndex] = node;
	maxIndex++;
}