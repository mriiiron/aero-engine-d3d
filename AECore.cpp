#include <d3d11_1.h>
#include "AEcore.h"

AEResourceTable rTable;
AEConstantTable<AEObject, 100> oTable;
AEBackgroundLibrary bgLib;
AEFontLibrary fontLib;
AEParticleSystem ptclSys;
AESceneManager sceneManager;
AECamera camera;
AEHashedTable<AEAI, 100> aiTable;