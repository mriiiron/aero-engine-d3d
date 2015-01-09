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
	case 3:
		return new WLFColossusSmash();
	case 4:
		return new WLFThunderClap();
	default:
		return nullptr;
	}
}


VOID WLFMortalStrike::takeEffect() {
	target->takeDamage(5);
	target->addBuff(new WLFBuffRend(1500), caster);
}


VOID WLFOverpower::takeEffect() {
	target->takeDamage(3);
}


VOID WLFColossusSmash::takeEffect() {
	target->takeDamage(3);
	target->addBuff(new WLFBuffArmorCrack(360), caster);
}


VOID WLFThunderClap::takeEffect() {
	target->takeDamage(1);
	target->addBuff(new WLFBuffSlowDown(600), caster);
}