#include "AEroEngine.h"
#include "WLFBuff.h"
#include "WLFSprite.h"
#include "WLFScene.h"
#include "WLFAI.h"

extern AEConstantTable<AEObject>			ae_ObjectTable;


WLFFloatingTextAI::WLFFloatingTextAI(AESprite* _host) : AEAI(_host) {
	time = 60;
	floatSpeed = -0.5f;
}

VOID WLFFloatingTextAI::execute() {
	host->setVy(floatSpeed);
	time--;
	if (time <= 0) {
		host->remove();
	}
}