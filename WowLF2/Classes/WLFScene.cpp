#include <dinput.h>

#include "AEroEngine.h"
#include "WLFFileReader.h"
#include "WLFAI.h"
#include "WLFAnimation.h"
#include "WLFSprite.h"
#include "WLFScene.h"

extern AEConstantTable<AEResource>			ae_ResourceTable;
extern AEConstantTable<AEObject>			ae_ObjectTable;
extern AEBackgroundLibrary					ae_BGLibrary;
extern AESceneManager						ae_SceneManager;
extern AECamera								ae_Camera;

const FLOAT WLFShrineCaveScene::GRAVITY = 0.2f;


VOID WLFShrineCaveScene::addObject(INT oid, std::string fileName, std::string objName, AEObjType objType) {
	AERO_OBJECT_DESC descObj;
	descObj.oid = oid;
	descObj.name = objName;
	descObj.otype = objType;
	AEObject* obj = new AEObject(descObj);
	WLFDataFileReader::readObject(fileName, obj);
	ae_ObjectTable.addAt(oid, obj);
}

WLFShrineCaveScene::WLFShrineCaveScene(INT spriteTableSize) : AEScene(spriteTableSize) {

	addObject(0, "Resources\\warrior_deep.txt", "Warrior Deep", OBJ_CHARACTER);
	addObject(1, "Resources\\dummy_bandit.txt", "Dummy Bandit", OBJ_CHARACTER);
	addObject(10, "Resources\\effect_hit_slash.txt", "Slash Effect", OBJ_EFFECT);
	addObject(15, "Resources\\effect_blood.txt", "Blood Effect", OBJ_EFFECT);
	addObject(40, "Resources\\ui.txt", "UI", OBJ_EFFECT);

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
	WLFCharacter* deep = new WLFCharacter(descSpr);
	addSprite(deep);

	// Set player
	player = deep;

	// Set camera focusing on player
	ae_Camera.setFocus(player->getCx(), 0.0f);

	// Create Dummy
	descSpr.obj = ae_ObjectTable.getItem(1);
	descSpr.team = 1;
	descSpr.action = 0;
	descSpr.flip = SpriteEffects_FlipHorizontally;
	descSpr.cx = 210.0f;
	descSpr.cy = 0.0f;  // 150.0f;
	descSpr.layerDepth = 0.01f;
	WLFCharacter* bandit = new WLFCharacter(descSpr);
	addSprite(bandit);

	// Create player namepad
	descSpr.obj = ae_ObjectTable.getItem(40);
	descSpr.team = 1;
	descSpr.action = 0;
	descSpr.flip = SpriteEffects_None;
	descSpr.cx = -300.0f;
	descSpr.cy = -220.0f;
	descSpr.layerDepth = 0.01f;
	AESprite* deep_namepad = new AESprite(descSpr);
	addSpriteForHUD(deep_namepad);

	descSpr.action = 2;
	descSpr.cx = deep_namepad->getCx();
	descSpr.cy = deep_namepad->getCy();
	AESprite* deep_namepad_face = new AESprite(descSpr);
	addSpriteForHUD(deep_namepad_face);

	descSpr.action = 1;
	AESprite* deep_namepad_hpbar = new AESprite(descSpr);
	addSpriteForHUD(deep_namepad_hpbar);


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
		if (keyStateBuffer[DIK_W] & 0x80) {
			player->toStand();
		}
		else {

		}
		if (keyStateBuffer[DIK_S] & 0x80) {
			player->toBattleStance();
		}
		else {

		}
		if (keyStateBuffer[DIK_A] & 0x80) {
			player->keyDown(WLFCharacter::KEY_LEFT);
		}
		else {
			player->keyUp(WLFCharacter::KEY_LEFT);
		}
		if (keyStateBuffer[DIK_D] & 0x80) {
			player->keyDown(WLFCharacter::KEY_RIGHT);
		}
		else {
			player->keyUp(WLFCharacter::KEY_RIGHT);
		}
		if (keyStateBuffer[DIK_J] & 0x80) {
			if (!player->isKeyPressed(WLFCharacter::KEY_ATTACK_1)) {
				player->attack_1();
			}
			player->keyDown(WLFCharacter::KEY_ATTACK_1);
		}
		else {
			player->keyUp(WLFCharacter::KEY_ATTACK_1);
		}
		if (keyStateBuffer[DIK_K] & 0x80) {
			if (!player->isKeyPressed(WLFCharacter::KEY_ATTACK_2)) {
				player->attack_2();
			}
			else {
				INT a = 1;
			}
			player->keyDown(WLFCharacter::KEY_ATTACK_2);
		}
		else {
			player->keyUp(WLFCharacter::KEY_ATTACK_2);
		}
		if (keyStateBuffer[DIK_TAB] & 0x80) {
			if (!player->isKeyPressed(WLFCharacter::KEY_CHANGE_TARGET)) {
				player->changeTarget();
			}
			else {
				INT a = 1;
			}
			player->keyDown(WLFCharacter::KEY_CHANGE_TARGET);
		}
		else {
			player->keyUp(WLFCharacter::KEY_CHANGE_TARGET);
		}
	}
	if (keyStateBuffer[DIK_L] & 0x80) {
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