#include <d3d11_1.h>
#include "AEObject.h"

AEObject::AEObject(AERO_OBJECT_DESC desc, AEAnimation** _animTable) {
	oid = desc.oid;
	name = desc.name;
	for (INT i = 0; i < MAX_OBJECT_ACTIONS; i++) {
		animTable[MAX_OBJECT_ACTIONS] = nullptr;
	}
	otype = desc.otype;
}