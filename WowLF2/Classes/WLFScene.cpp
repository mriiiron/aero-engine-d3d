#include <dinput.h>

#include "AEroEngine.h"
#include "WLFFileReader.h"
#include "WLFAI.h"
#include "WLFAnimation.h"
#include "WLFBuff.h"
#include "WLFSprite.h"
#include "WLFSpell.h"
#include "WLFScene.h"

extern AEConstantTable<AEResource>			ae_ResourceTable;
extern AEConstantTable<AEObject>			ae_ObjectTable;
extern AEBackgroundLibrary					ae_BGLibrary;
extern AESceneManager						ae_SceneManager;
extern AECamera								ae_Camera;

const FLOAT WLFShrineCaveScene::GRAVITY = 0.2f;


WLFShrineCaveScene::WLFShrineCaveScene(INT spriteTableSize) : AEScene(spriteTableSize) {

	// Set key

	WLFShrineCaveScene::dik_left = DIK_A;
	WLFShrineCaveScene::dik_right = DIK_D;
	WLFShrineCaveScene::dik_up = DIK_W;
	WLFShrineCaveScene::dik_down = DIK_S;
	WLFShrineCaveScene::dik_attack_a = DIK_J;
	WLFShrineCaveScene::dik_attack_b = DIK_K;
	WLFShrineCaveScene::dik_jump = DIK_L;
	WLFShrineCaveScene::dik_special = DIK_I;

	// Load objects

	loadObject(0, "Resources\\warrior_deep.txt", "Warrior Deep", OBJ_CHARACTER);
	loadObject(1, "Resources\\dummy_bandit.txt", "Dummy Bandit", OBJ_CHARACTER);
	loadObject(10, "Resources\\effects.txt", "Effect", OBJ_EFFECT);
	loadObject(11, "Resources\\icons.txt", "Icon", OBJ_EFFECT);
	loadObject(40, "Resources\\ui.txt", "UI", OBJ_EFFECT);

	// Create Shrine Cave BG

	AEBGLayerFrame* bgLayerFrame = new AEBGLayerFrame(ae_ResourceTable.getItem(20), 0);
	AERO_BGLAYERANIM_DESC descBGLayerAnim;
	descBGLayerAnim.frameCount = 1;
	AEBGLayerAnim* bgLayerAnim = new AEBGLayerAnim(descBGLayerAnim);
	bgLayerAnim->addFrame(0, bgLayerFrame, 1000);

	AERO_BACKGROUND_DESC descBG;
	descBG.name = "Shrine Cave";
	AEBackground* bg_shrine_cave = new AEBackground(descBG);
	bg_shrine_cave->addAnimAt(0, bgLayerAnim);

	AERO_BGLAYER_DESC descBGLayer;
	descBGLayer.depth = 0;
	descBGLayer.locX = -750.0f;
	descBGLayer.locY = -240.0f;
	descBGLayer.width = 1500;
	descBGLayer.height = 480;
	AEBGLayer* bgLayer = new AEBGLayer(descBGLayer);

	AEBGAnimRef* animRef = new AEBGAnimRef(0, 0, 0);
	bgLayer->addAnimRef(animRef);

	bg_shrine_cave->addLayer(bgLayer);
	ae_BGLibrary.add(bg_shrine_cave);
	this->setBackground(bg_shrine_cave);

	// Create Platforms
	AEPlatform* platform_floor = new AEPlatform(14);
	platform_floor->addNode({ descBGLayer.locX + 0.0f, descBGLayer.locY + 352.0f });
	platform_floor->addNode({ descBGLayer.locX + 236.0f, descBGLayer.locY + 352.0f });
	platform_floor->addNode({ descBGLayer.locX + 287.0f, descBGLayer.locY + 313.0f });
	platform_floor->addNode({ descBGLayer.locX + 471.0f, descBGLayer.locY + 310.0f });
	platform_floor->addNode({ descBGLayer.locX + 552.0f, descBGLayer.locY + 368.0f });
	platform_floor->addNode({ descBGLayer.locX + 643.0f, descBGLayer.locY + 390.0f });
	platform_floor->addNode({ descBGLayer.locX + 867.0f, descBGLayer.locY + 390.0f });
	platform_floor->addNode({ descBGLayer.locX + 946.0f, descBGLayer.locY + 344.0f });
	platform_floor->addNode({ descBGLayer.locX + 990.0f, descBGLayer.locY + 344.0f });
	platform_floor->addNode({ descBGLayer.locX + 1046.0f, descBGLayer.locY + 382.0f });
	platform_floor->addNode({ descBGLayer.locX + 1158.0f, descBGLayer.locY + 382.0f });
	platform_floor->addNode({ descBGLayer.locX + 1370.0f, descBGLayer.locY + 318.0f });
	platform_floor->addNode({ descBGLayer.locX + 1433.0f, descBGLayer.locY + 278.0f });
	platform_floor->addNode({ descBGLayer.locX + 1500.0f, descBGLayer.locY + 264.0f });
	platform_floor->calcAngles();

	// Apply platforms
	AEHashedTable<AEPlatform>* platforms_shrine_cave = new AEHashedTable<AEPlatform>(10);
	platforms_shrine_cave->add(platform_floor);
	this->setPlatformTable(platforms_shrine_cave);

	// Add HUD
	hud = new AEHeadUpDisplay(100);

}

VOID WLFShrineCaveScene::initialize() {

	// Create Player
	AERO_SPRITE_DESC descSpr;
	descSpr.obj = ae_ObjectTable.getItem(0);
	descSpr.team = 0;
	descSpr.action = 0;
	descSpr.flip = SpriteEffects_None;
	descSpr.cx = -200.0f;
	descSpr.cy = 0.0f;
	descSpr.layerDepth = 0.001f;
	WLFWarrior* deep = new WLFWarrior(descSpr);
	addSprite(deep);

	// Set player
	player = deep;

	// Set camera focusing on player
	ae_Camera.setFocus(player->getCx(), 0.0f);

	// Create Dummy #1
	descSpr.obj = ae_ObjectTable.getItem(1);
	descSpr.team = 1;
	descSpr.action = 0;
	descSpr.flip = SpriteEffects_FlipHorizontally;
	descSpr.cx = 210.0f;
	descSpr.cy = 0.0f;  // 150.0f;
	descSpr.layerDepth = 0.01f;
	WLFCharacter* bandit1 = new WLFCharacter(descSpr);
	addSprite(bandit1);

	// Create Dummy #2
	descSpr.cx = 0.0f;
	descSpr.cy = 0.0f;  // 150.0f;
	WLFCharacter* bandit2 = new WLFCharacter(descSpr);
	addSprite(bandit2);

	// Create player namepad
	addNamepadToHUD(deep, 10, NAMEPAD_SLOT_PLAYER);

}

VOID WLFShrineCaveScene::loadObject(INT oid, std::string fileName, std::string objName, AEObjType objType) {
	AERO_OBJECT_DESC descObj;
	descObj.oid = oid;
	descObj.name = objName;
	descObj.otype = objType;
	AEObject* obj = new AEObject(descObj);
	WLFDataFileReader::readObject(fileName, obj);
	ae_ObjectTable.addAt(oid, obj);
}

VOID WLFShrineCaveScene::addNamepadToHUD(WLFCharacter* character, INT portraitIndex, INT slot) {

	AERO_SPRITE_DESC descSpr;
	descSpr.obj = ae_ObjectTable.getItem(40);
	descSpr.team = character->getTeam();
	descSpr.action = 0;
	descSpr.flip = SpriteEffects_None;
	if (slot == NAMEPAD_SLOT_PLAYER) {
		descSpr.cx = -300.0f;
	}
	else {
		descSpr.cx = -150.0f;
	}
	descSpr.cy = -220.0f;
	descSpr.layerDepth = 0.01f;
	AESprite* namepad = new AESprite(descSpr);
	addSpriteForHUD(namepad);

	descSpr.action = portraitIndex;
	descSpr.cx = namepad->getCx();
	descSpr.cy = namepad->getCy();
	AESprite* portrait = new AESprite(descSpr);
	addSpriteForHUD(portrait);

	descSpr.action = 1;
	WLFBar* hpbar = new WLFBar(descSpr, character, WLFBar::TYPE_HP);
	addSpriteForHUD(hpbar);

	descSpr.action = 3;
	WLFBar* energybar = new WLFBar(descSpr, character, WLFBar::TYPE_ENERGY);
	addSpriteForHUD(energybar);

	character->setHUDItemIndexes(namepad->getIndex(), portrait->getIndex(), hpbar->getIndex(), energybar->getIndex());

}

VOID WLFShrineCaveScene::removeNamepadFromHUD(WLFCharacter* character) {
	WLFCharacterHUDItemIndexes namepadItems = character->getHUDItemIndexes();
	hud->getSpriteTable()->getItem(namepadItems.namepad)->remove();
	hud->getSpriteTable()->getItem(namepadItems.portrait)->remove();
	hud->getSpriteTable()->getItem(namepadItems.bar_hp)->remove();
	hud->getSpriteTable()->getItem(namepadItems.bar_energy)->remove();
}

VOID WLFShrineCaveScene::update() {
	processCollision();
	AEScene::update();
}

VOID WLFShrineCaveScene::render(INT renderMode) {
	ae_Camera.setFocus(player->getCx(), 0.0f);
	AEScene::render(renderMode);
}

VOID WLFShrineCaveScene::processInput() {
	if (!isPaused) {
		if (keyStateBuffer[dik_up] & 0x80) {
			if (player->getAction() == 1) {
				player->toStand();
			}
		}
		else {

		}
		if (keyStateBuffer[dik_down] & 0x80) {
			if (player->getAction() == 0) {
				player->toBattleStance();
			}
		}
		else {

		}
		if (keyStateBuffer[dik_left] & 0x80) {
			player->keyDown(WLFCharacter::KEY_LEFT);
		}
		else {
			player->keyUp(WLFCharacter::KEY_LEFT);
		}
		if (keyStateBuffer[dik_right] & 0x80) {
			player->keyDown(WLFCharacter::KEY_RIGHT);
		}
		else {
			player->keyUp(WLFCharacter::KEY_RIGHT);
		}
		if (keyStateBuffer[dik_attack_a] & 0x80) {
			if (!player->isKeyPressed(WLFCharacter::KEY_ATTACK_A) && player->getAction() == 1) {
				player->mortalStrike();
			}
			player->keyDown(WLFCharacter::KEY_ATTACK_A);
		}
		else {
			player->keyUp(WLFCharacter::KEY_ATTACK_A);
		}
		if (keyStateBuffer[dik_attack_b] & 0x80) {
			if (!player->isKeyPressed(WLFCharacter::KEY_ATTACK_B) && player->getAction() == 1) {
				player->overpower();
			}
			player->keyDown(WLFCharacter::KEY_ATTACK_B);
		}
		else {
			player->keyUp(WLFCharacter::KEY_ATTACK_B);
		}
		if (keyStateBuffer[dik_special] & 0x80) {
			if (!player->isKeyPressed(WLFCharacter::KEY_SPECIAL) && player->hasTarget() && (player->getAction() >= 0 || player->getAction() <= 5)) {
				player->charge();
			}
			player->keyDown(WLFCharacter::KEY_SPECIAL);
		}
		else {
			player->keyUp(WLFCharacter::KEY_SPECIAL);
		}
		if (keyStateBuffer[DIK_TAB] & 0x80) {
			if (!player->isKeyPressed(WLFCharacter::KEY_CHANGE_TARGET)) {
				player->changeTarget();
			}
			player->keyDown(WLFCharacter::KEY_CHANGE_TARGET);
		}
		else {
			player->keyUp(WLFCharacter::KEY_CHANGE_TARGET);
		}
	}
	if (keyStateBuffer[dik_jump] & 0x80) {
		INT a = 1;
	}
	if (keyStateBuffer[DIK_RETURN] & 0x80) {
		if (!isPauseKeyPressed) {
			isPauseKeyPressed = TRUE;
			togglePause();
		}
	}
	else {
		isPauseKeyPressed = FALSE;
	}
}

VOID WLFShrineCaveScene::processCollision() {
	INT spriteCount = spriteTable->getHashCount();
	for (INT i = 0; i < spriteCount; i++) {
		AESprite* sprite1 = spriteTable->getItemByHash(i);
		WLFAnimation* anim1 = (WLFAnimation*)(sprite1->getCurrentAnimation());
		WLFAttackJudgeArea* attackJudge = anim1->getAttackJudge(sprite1->getFrameNum());
		if (attackJudge == nullptr || dynamic_cast<WLFCharacter*>(sprite1)->isAttackLocked()) {
			continue;
		}
		for (INT j = 0; j < spriteCount; j++) {
			AESprite* sprite2 = spriteTable->getItemByHash(j);
			if (i == j || sprite1->getTeam() == sprite2->getTeam()) {
				continue;
			}
			WLFAnimation* anim2 = (WLFAnimation*)(sprite2->getCurrentAnimation());
			WLFBodyJudgeArea* bodyJudge = anim2->getBodyJudge(sprite2->getFrameNum());
			if (bodyJudge == nullptr) {
				continue;
			}
			AEFrame* frame1 = anim1->getFrame(sprite1->getFrameNum());
			AEFrame* frame2 = anim2->getFrame(sprite2->getFrameNum());
			FLOAT judgeRectOffsetX1 = (sprite1->getFlip() == SpriteEffects_None ? sprite1->getCx() - frame1->getCenterx() : sprite1->getCx() + frame1->getCenterx() - attackJudge->rect.left - attackJudge->rect.right);
			FLOAT judgeRectOffsetX2 = (sprite2->getFlip() == SpriteEffects_None ? sprite2->getCx() - frame2->getCenterx() : sprite2->getCx() + frame2->getCenterx() - bodyJudge->rect.left - bodyJudge->rect.right);
			FLOAT judgeRectOffsetY1 = sprite1->getCy() - frame1->getCentery();
			FLOAT judgeRectOffsetY2 = sprite2->getCy() - frame2->getCentery();
			AECollisionResult collisionResult = AENSCollision::rectAndRect(
				AENSMath::moveRect(attackJudge->rect, judgeRectOffsetX1, judgeRectOffsetY1),
				AENSMath::moveRect(bodyJudge->rect, judgeRectOffsetX2, judgeRectOffsetY2)
			);
			if (collisionResult.isCollided) {
				
				if (attackJudge->spellID > 0) {
					WLFSpell* spell = WLFSpell::getSpellByID(attackJudge->spellID);
					if (spell == nullptr) {
						AENSGameControl::warning("Warning: Unknown Spell ID.");
					}
					else {
						spell->setCaster(dynamic_cast<WLFCharacter*>(sprite1));
						spell->setTarget(dynamic_cast<WLFCharacter*>(sprite2));
						spell->takeEffect();
					}
					delete spell;
				}

				AESprite* spark;
				AERO_SPRITE_DESC descSpr;
				descSpr.obj = ae_ObjectTable.getItem(10);  // Slash Effect
				descSpr.team = -1;  // Neutral
				descSpr.action = attackJudge->effect;
				descSpr.cx = collisionResult.point.x;
				descSpr.cy = collisionResult.point.y;
				descSpr.layerDepth = 0.0f;

				dynamic_cast<WLFCharacter*>(sprite1)->setAttackLock(TRUE);
				if (sprite1->getFlip() != sprite2->getFlip()) {
					sprite2->changeAction(AENSMath::randomIntBetween(WLFCharacter::ACTION_HIT_FRONT_LOWER, WLFCharacter::ACTION_HIT_FRONT_UPPER));
					descSpr.flip = SpriteEffects_None;
					spark = new AESprite(descSpr);
					spark->rotateDeg(attackJudge->angle);
				}
				else {
					sprite2->changeAction(AENSMath::randomIntBetween(WLFCharacter::ACTION_HIT_BACK_LOWER, WLFCharacter::ACTION_HIT_BACK_UPPER));
					descSpr.flip = SpriteEffects_FlipHorizontally;
					spark = new AESprite(descSpr);
					spark->rotateDeg(-attackJudge->angle);
				}

				addSprite(spark);

			}
		}
	}
}