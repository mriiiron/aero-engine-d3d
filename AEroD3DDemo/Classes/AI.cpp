#include "AEroEngine.h"
#include "AI.h"

extern AEConstantTable<AEObject>			ae_ObjectTable;

RocketAI::RocketAI(AESprite* _self) : AEAI(_self) {
	smokeCooldown = 15;
	host->setVy(-5.0f);
	host->setAy(0.1f);
}

VOID RocketAI::execute() {
	if (smokeCooldown <= 0) {
		host->setVy(-10.0f);
		host->setAy(0.0f);
		AERO_SPRITE_DESC descSpr;
		descSpr.obj = ae_ObjectTable.getItem(3);
		descSpr.team = 0;
		descSpr.action = 0;
		descSpr.facing = SpriteEffects_None;
		descSpr.cx = host->getCx();
		descSpr.cy = host->getCy();
		AESprite* spr_smoke = new AESprite(descSpr);
		XMFLOAT2 randomSpeed = AENSMath::randomPointWithinCircle(2.0f);
		spr_smoke->setVx(randomSpeed.x);
		spr_smoke->setVy(randomSpeed.y);
		host->getScene()->addSprite(spr_smoke);
		smokeCooldown = 0;
	}
	else {
		smokeCooldown--;
	}
}