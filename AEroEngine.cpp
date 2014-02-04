#include "AEroEngine.h"

AEConstantTable<AEResource, 100>			resourceTable;
AEConstantTable<AEObject, 100>				objectTable;
AEHashedTable<AEAI, 100>					aiTable;
AEHashedTable<AEParticleEmitter, 1000>		particleTable;
AEBackgroundLibrary							bgLib;
AESceneManager								sceneManager;
AECamera									camera;