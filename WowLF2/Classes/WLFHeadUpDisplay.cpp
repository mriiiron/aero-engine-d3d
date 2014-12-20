#include "AEroEngine.h"
#include "WLFBuff.h"
#include "WLFSprite.h"
#include "WLFHeadUpDisplay.h"

extern AEConstantTable<AEObject>			ae_ObjectTable;

WLFHeadUpDisplay::WLFHeadUpDisplay(INT spriteTableSize) : AEHeadUpDisplay(spriteTableSize) {
	for (INT i = 0; i < 2; i++) {
		items[i].namepadIndex = items[i].portraitIndex = items[i].barHPIndex = items[i].barEnergyIndex = 0;
		items[i].charRef = nullptr;
		for (INT j = 0; j < 20; j++) {
			items[i].buffIndex[j] = -1;
		}
		items[i].buffCount = 0;
	}
}

VOID WLFHeadUpDisplay::update() {
	AEHeadUpDisplay::update();
	for (INT i = 0; i < 2; i++) {
		if (items[i].charRef != nullptr) {
			for (INT j = 0; j < items[i].buffCount; j++) {
				spriteTable->getItem(items[i].buffIndex[j])->remove();
				items[i].buffIndex[j] = -1;
			}
			AEHashedTable<WLFBuff>* buffTable = items[i].charRef->getBuffTable();
			items[i].buffCount = 0;
			for (INT iHash = 0; iHash < buffTable->getHashCount(); iHash++) {
				WLFBuff* buff = buffTable->getItemByHash(iHash);
				AERO_SPRITE_DESC descSpr;
				descSpr.obj = ae_ObjectTable.getItem(11);
				descSpr.team = -1;
				descSpr.action = buff->getIconIndex();
				descSpr.flip = SpriteEffects_None;
				descSpr.cx = -285.0f + 150.0f * i;
				descSpr.cy = -170.0f + 20.0f * iHash;
				descSpr.layerDepth = 0.01f;
				WLFBuffIcon* buffIcon = new WLFBuffIcon(descSpr, buff);
				scene->addSpriteForHUD(buffIcon);
				items[i].buffIndex[items[i].buffCount] = buffIcon->getIndex();
				items[i].buffCount++;
			}
		}
	}
}

VOID WLFHeadUpDisplay::removeNamepad(INT slot) {
	spriteTable->getItem(items[slot].namepadIndex)->remove();
	spriteTable->getItem(items[slot].portraitIndex)->remove();
	spriteTable->getItem(items[slot].barHPIndex)->remove();
	spriteTable->getItem(items[slot].barEnergyIndex)->remove();
	for (INT j = 0; j < items[slot].buffCount; j++) {
		spriteTable->getItem(items[slot].buffIndex[j])->remove();
		items[slot].buffIndex[j] = -1;
	}
	items[slot].buffCount = 0;
}

VOID WLFHeadUpDisplay::setNamepad(INT slot, WLFCharacter* character) {

	if (items[slot].charRef != nullptr) {
		removeNamepad(slot);
	}
	items[slot].charRef = character;

	AERO_SPRITE_DESC descSpr;
	descSpr.obj = ae_ObjectTable.getItem(40);
	descSpr.team = -1;
	descSpr.action = 0;
	descSpr.flip = SpriteEffects_None;
	descSpr.cx = -300.0f + 150.0f * slot;
	descSpr.cy = -220.0f;
	descSpr.layerDepth = 0.01f;
	AESprite* namepad = new AESprite(descSpr);
	scene->addSpriteForHUD(namepad);

	descSpr.action = character->getPortraitIndex();
	descSpr.cx = namepad->getCx();
	descSpr.cy = namepad->getCy();
	AESprite* portrait = new AESprite(descSpr);
	scene->addSpriteForHUD(portrait);

	descSpr.action = 1;
	WLFBar* hpbar = new WLFBar(descSpr, character, WLFBar::TYPE_HP);
	scene->addSpriteForHUD(hpbar);

	descSpr.action = 3;
	WLFBar* energybar = new WLFBar(descSpr, character, WLFBar::TYPE_ENERGY);
	scene->addSpriteForHUD(energybar);

	items[slot].namepadIndex = namepad->getIndex();
	items[slot].portraitIndex = portrait->getIndex();
	items[slot].barHPIndex = hpbar->getIndex();
	items[slot].barEnergyIndex = energybar->getIndex();

}