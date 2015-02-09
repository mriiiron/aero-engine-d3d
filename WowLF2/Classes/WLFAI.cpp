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


WLFBanditAI::WLFBanditAI(AESprite* _host) : AEAI(_host) {

}

VOID WLFBanditAI::execute() {
	WLFCharacter* bandit = dynamic_cast<WLFCharacter*>(host);
	WLFShrineCaveScene* scene = dynamic_cast<WLFShrineCaveScene*>(bandit->getScene());
	if (bandit->getTarget() == nullptr) {
		bandit->setTarget(scene->getPlayer());
	}
	else {
		if (fabs(bandit->getCx() - bandit->getTarget()->getCx()) < 80.0f){
			bandit->applyMoveInput(WLFCharacter::MOVE_A);
		}
	}
}