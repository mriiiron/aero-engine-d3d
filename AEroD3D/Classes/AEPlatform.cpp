#include <d3d11_1.h>

#include "AEMath.h"
#include "AETable.h"
#include "AEPlatform.h"

AEPlatform::AEPlatform(INT _nodeCount) {
    maxIndex = 0;
    nodeCount = _nodeCount;
    nodeTable = new XMFLOAT2[nodeCount];
    angleTable = new FLOAT[nodeCount - 1];
}

AEPlatform::~AEPlatform() {
    delete[] nodeTable;
    delete[] angleTable;
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

FLOAT AEPlatform::getAngleRad(INT index) {
    if (index < 0) {
        return angleTable[0];
    }
    else if (index >= nodeCount - 1) {
        return angleTable[nodeCount - 2];
    }
    else {
        return angleTable[index];
    }
}

VOID AEPlatform::addNode(XMFLOAT2 node) {
    if (maxIndex >= nodeCount) {
        return;
    }
    nodeTable[maxIndex] = node;
    maxIndex++;
}

VOID AEPlatform::calcAngles() {
    for (INT i = 0; i < nodeCount - 1; i++) {
        if (nodeTable[i + 1].x == nodeTable[i].x) {
            angleTable[i] = nodeTable[i + 1].y >= nodeTable[i].y ? AENSMath::PI / 2.0f : -AENSMath::PI / 2.0f;
            continue;
        }
        angleTable[i] = atanf((nodeTable[i + 1].y - nodeTable[i].y) / (nodeTable[i + 1].x - nodeTable[i].x));
        if (nodeTable[i + 1].x < nodeTable[i].x) {
            angleTable[i] += (angleTable[i] < 0 ? AENSMath::PI : -AENSMath::PI);
        }
    }
}