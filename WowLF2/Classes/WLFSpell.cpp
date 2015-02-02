#include "AEroEngine.h"
#include "WLFBuff.h"
#include "WLFSprite.h"
#include "WLFAI.h"
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
	case 5:
		return new WLFSlam();
	default:
		return nullptr;
	}
}


VOID WLFMortalStrike::takeEffect() {
	INT damage = 50 + AENSMath::randomIntBetween(-5, 5);
	if (target->hasBuff("Armor Crack")) {
		damage = (INT)(damage * 1.4f);
	}
	target->takeDamage(damage);
	target->addBuff(new WLFBuffRend(1500), caster);
	AERO_SPRITE_DESC descSpr;
	descSpr.cx = target->getCx() - 50.0f;
	descSpr.cy = target->getCy() - 100.0f;
	WLFFloatingText* text = new WLFFloatingText(descSpr, "Mortal Strike " + std::to_string(damage));
	target->getScene()->addSprite(text);
	text->setAI(new WLFFloatingTextAI(text));
}


VOID WLFOverpower::takeEffect() {
	INT damage = 30 + AENSMath::randomIntBetween(-3, 3);
	if (target->hasBuff("Armor Crack")) {
		damage = (INT)(damage * 1.4f);
	}
	target->takeDamage(damage);
	AERO_SPRITE_DESC descSpr;
	descSpr.cx = target->getCx() - 50.0f;
	descSpr.cy = target->getCy() - 100.0f;
	WLFFloatingText* text = new WLFFloatingText(descSpr, "Overpower " + std::to_string(damage));
	target->getScene()->addSprite(text);
	text->setAI(new WLFFloatingTextAI(text));
}


VOID WLFColossusSmash::takeEffect() {
	INT damage = 30 + AENSMath::randomIntBetween(-3, 3);
	if (target->hasBuff("Armor Crack")) {
		damage = (INT)(damage * 1.4f);
	}
	target->takeDamage(damage);
	target->addBuff(new WLFBuffArmorCrack(360), caster);
	AERO_SPRITE_DESC descSpr;
	descSpr.cx = target->getCx() - 50.0f;
	descSpr.cy = target->getCy() - 100.0f;
	WLFFloatingText* text = new WLFFloatingText(descSpr, "Colossus Smash " + std::to_string(damage));
	target->getScene()->addSprite(text);
	text->setAI(new WLFFloatingTextAI(text));
}


VOID WLFThunderClap::takeEffect() {
	INT damage = 20 + AENSMath::randomIntBetween(-2, 2);
	target->takeDamage(damage);
	target->addBuff(new WLFBuffSlowDown(600), caster);
	AERO_SPRITE_DESC descSpr;
	descSpr.cx = target->getCx() - 50.0f;
	descSpr.cy = target->getCy() - 100.0f;
	WLFFloatingText* text = new WLFFloatingText(descSpr, "Thunder Clap " + std::to_string(damage));
	target->getScene()->addSprite(text);
	text->setAI(new WLFFloatingTextAI(text));
}


VOID WLFSlam::takeEffect() {
	INT damage = 75 + AENSMath::randomIntBetween(-7, 7);
	if (target->hasBuff("Armor Crack")) {
		damage = (INT)(damage * 1.5f);
	}
	target->takeDamage(damage);
	AERO_SPRITE_DESC descSpr;
	descSpr.cx = target->getCx() - 50.0f;
	descSpr.cy = target->getCy() - 100.0f;
	WLFFloatingText* text = new WLFFloatingText(descSpr, "Slam " + std::to_string(damage));
	target->getScene()->addSprite(text);
	text->setAI(new WLFFloatingTextAI(text));
}