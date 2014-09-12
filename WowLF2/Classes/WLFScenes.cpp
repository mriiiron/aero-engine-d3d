#include <dinput.h>

#include "AEroEngine.h"
#include "WLFFileReader.h"
#include "WLFAI.h"
#include "WLFAnimation.h"
#include "WLFSprites.h"
#include "WLFScenes.h"

extern AEConstantTable<AEResource>			ae_ResourceTable;
extern AEConstantTable<AEObject>			ae_ObjectTable;
extern AEBackgroundLibrary					ae_BGLibrary;
extern AESceneManager						ae_SceneManager;


const FLOAT WLFShrineCaveScene::GRAVITY = 0.2f;

WLFShrineCaveScene::WLFShrineCaveScene(AEBackground* _bg, AEHashedTable<AEPlatform>* _platformTable, AEHashedTable<AESprite>* _spriteTable, AEHeadUpDisplay* _hud) : AEScene(_bg, _platformTable, _spriteTable, _hud) {

	// Create Warrior Deep

	AERO_OBJECT_DESC descObj;
	descObj.oid = 0;
	descObj.name = "Warrior Deep";
	descObj.otype = OBJ_CHARACTER;
	AEObject* deep_obj = new AEObject(descObj);

	WLFObjectDataFileReader::read("Resources\\warrior_deep.txt", deep_obj);
	ae_ObjectTable.addAt(descObj.oid, deep_obj);

	// Create Dummy Bandit

	descObj.oid = 1;
	descObj.name = "Dummy Bandit";
	descObj.otype = OBJ_CHARACTER;
	AEObject* bandit_obj = new AEObject(descObj);

	WLFObjectDataFileReader::read("Resources\\dummy_bandit.txt", bandit_obj);
	ae_ObjectTable.addAt(descObj.oid, bandit_obj);

	// Create Shrine Cave BG

	AEBGLayerFrame* bgLayerFrame = new AEBGLayerFrame(ae_ResourceTable.getItem(10), 0);
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
	this->player = deep;

	// Create Dummy
	descSpr.obj = ae_ObjectTable.getItem(1);
	descSpr.team = 1;
	descSpr.action = 0;
	descSpr.flip = SpriteEffects_FlipHorizontally;
	descSpr.cx = 90.0f;
	descSpr.cy = 150.0f;
	descSpr.layerDepth = 0.01f;
	WLFDummy* bandit = new WLFDummy(descSpr);
	addSprite(bandit);

}

VOID WLFShrineCaveScene::update() {
	AEScene::update();
}

VOID WLFShrineCaveScene::render() {
	AEScene::render();
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
			player->isLeftKeyPressed = TRUE;
		}
		else {
			player->isLeftKeyPressed = FALSE;
		}
		if (keyStateBuffer[DIK_D] & 0x80) {
			player->isRightKeyPressed = TRUE;
		}
		else {
			player->isRightKeyPressed = FALSE;
		}
		if (keyStateBuffer[DIK_J] & 0x80) {
			player->attack_1();
		}
		if (keyStateBuffer[DIK_K] & 0x80) {
			player->attack_2();
		}
		if (keyStateBuffer[DIK_L] & 0x80) {
			INT a = 1;
		}
	}
}

VOID WLFShrineCaveScene::processCollision() {
	INT spriteCount = spriteTable->getHashCount() - 1;
	for (INT i = 0; i < spriteCount; i++) {
		AESprite* s1 = spriteTable->getItemByHash(i);
		WLFAnimation* anim1 = (WLFAnimation*)(s1->getAnimation());
		WLFJudgeAreaRect* judgeArea1 = anim1->getJudgeArea(s1->getFrameNum());
		if (judgeArea1 == nullptr || judgeArea1->type != WLFAnimation::JUDGE_TYPE_ATTACK) {
			continue;
		}
		for (INT j = 0; i < spriteCount; j++) {
			AESprite* s2 = spriteTable->getItemByHash(j);
			if (i == j || s1->getTeam() == s2->getTeam()) {
				continue;
			}
			WLFAnimation* anim2 = (WLFAnimation*)(s2->getAnimation());
			WLFJudgeAreaRect* judgeArea2 = anim2->getJudgeArea(s2->getFrameNum());
			if (judgeArea2 == nullptr || judgeArea2->type != WLFAnimation::JUDGE_TYPE_ON_HIT) {
				continue;
			}
			AECollisionResult collisionResult = AENSCollision::rectAndRect(judgeArea1->rect, judgeArea2->rect);
			if (collisionResult.isCollided) {

			}
		}
	}
}