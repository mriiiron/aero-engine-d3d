#include "AEroEngine.h"
#include "AI.h"

RocketAI::RocketAI(AESprite* _self) : AEAI(_self) {
	// ^^
}

VOID RocketAI::execute() {
	host->setVy(10.0f);
}