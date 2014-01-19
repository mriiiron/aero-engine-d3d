#include <d3d11_1.h>
#include "AEcore.h"

AEConstantTable<AEResource, 100> rTable;
AEConstantTable<AEObject, 100> oTable;
AEBackgroundLibrary bgLib;
AEParticleSystem ptclSys;
AESceneManager sceneManager;
AECamera camera;
AEHashedTable<AEAI, 100> aiTable;