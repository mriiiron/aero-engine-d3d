#include <d3d11_1.h>
#include <string>
#include "AEMath.h"
#include "AEResource.h"
#include "AETable.h"
#include "AEBackground.h"

AEBackground::AEBackground() {
	location.x = location.y = 0;
	layerCount = landformCount = 0;
}