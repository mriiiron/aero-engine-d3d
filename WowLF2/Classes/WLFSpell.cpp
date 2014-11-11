#include "AEroEngine.h"
#include "WLFBuff.h"
#include "WLFSprite.h"
#include "WLFSpell.h"


WLFSpell::WLFSpell() {
	caster = nullptr;
	target = nullptr;
}

WLFSpell* WLFSpell::getSpellByID(INT spellID) {
	switch (spellID) {
	case 1:
		return new WLFMortalStrike();
	case 2:
		return new WLFOverpower();
	default:
		return nullptr;
	}
}


VOID WLFMortalStrike::takeEffect() {
	target->takeDamage(5);
	target->addBuff(new WLFBuffRend(600));
}


VOID WLFOverpower::takeEffect() {
	target->takeDamage(3);
}