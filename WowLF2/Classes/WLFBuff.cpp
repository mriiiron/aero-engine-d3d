#include "AEroEngine.h"
#include "WLFBuff.h"
#include "WLFSprite.h"

extern AEConstantTable<AEObject>			ae_ObjectTable;


WLFBuff::WLFBuff(INT _time) {
	host = nullptr;
	name = "Unknown";
	timeRemain = _time;
	icon = nullptr;
	deadFlag = FALSE;
}


WLFBuffRend::WLFBuffRend(INT _time) : WLFBuff(_time) {
	name = "Rend";
	icon = ae_ObjectTable.getItem(11)->getAnim(0)->getFrame(0);
}

VOID WLFBuffRend::update() {
	if (timeRemain % 120 == 0) {
		host->takeDamage(1);
	}
	timeRemain--;
	if (timeRemain <= 0) {
		deadFlag = TRUE;
	}
}