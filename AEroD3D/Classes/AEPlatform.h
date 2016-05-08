#pragma once

#include <DirectXMath.h>

using namespace DirectX;


struct AERO_PLATFORM_DESC {

    INT pid;
    INT nodeCount;

    AERO_PLATFORM_DESC() {
        pid = 0;
        nodeCount = 0;
    }

};

class AEPlatform {

public:

    AEPlatform(AERO_PLATFORM_DESC desc);
    ~AEPlatform();

    INT getNodeCount() { return nodeCount; }
    INT getSegmentCount() { return nodeCount - 1; }
    XMFLOAT2 getNode(INT index);
    FLOAT getAngleRad(INT index);
    VOID addNode(XMFLOAT2 node);
    VOID calcAngles();

private:

    INT pid;
    INT maxIndex, nodeCount;
    XMFLOAT2* nodeTable;
    FLOAT* angleTable;

};