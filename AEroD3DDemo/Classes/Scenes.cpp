#include <dinput.h>

#include "AEroEngine.h"
#include "AI.h"
#include "Sprites.h"
#include "Scenes.h"

extern AEConstantTable<AEResource>			ae_ResourceTable;
extern AEConstantTable<AEObject>			ae_ObjectTable;
extern AEBackgroundLibrary					ae_BGLibrary;
extern AESceneManager						ae_SceneManager;

GeneralSampleScene::GeneralSampleScene(AEBackground* _bg, AEHashedTable<AEPlatform>* _platformTable, AEHashedTable<AESprite>* _spriteTable, AEHeadUpDisplay* _hud) : AEScene(_bg, _platformTable, _spriteTable, _hud) {
	
	// Frames for Gedama
	AERO_FRAME_DESC descFrame;
	descFrame.res = ae_ResourceTable.getItem(0);
	descFrame.centerx = 40;
	descFrame.centery = 60;
	descFrame.dvx = 0;
	descFrame.dvy = 0;
	descFrame.imgOffset = 0;
	descFrame.imgCells = 1;
	descFrame.shiftx = 0;
	descFrame.shifty = 0;
	AEFrame* gedama_frame_00 = new AEFrame(descFrame);
	descFrame.imgOffset = 1;
	AEFrame* gedama_frame_01 = new AEFrame(descFrame);
	descFrame.imgOffset = 2;
	AEFrame* gedama_frame_02 = new AEFrame(descFrame);
	descFrame.imgOffset = 1;
	AEFrame* gedama_frame_03 = new AEFrame(descFrame);
	descFrame.imgOffset = 10;
	AEFrame* gedama_frame_10 = new AEFrame(descFrame);
	descFrame.imgOffset = 11;
	AEFrame* gedama_frame_11 = new AEFrame(descFrame);
	descFrame.imgOffset = 12;
	AEFrame* gedama_frame_12 = new AEFrame(descFrame);

	// Idle animation for Gedama
	AERO_ANIMATION_DESC descAnim;
	descAnim.frameCount = 4;
	descAnim.isAnimLoop = TRUE;
	descAnim.next = AESprite::ACTION_NUM_DEAD;
	descAnim.state = 0;
	descAnim.timeToLive = -1;
	AEAnimation* gedama_anim_0 = new AEAnimation(descAnim);
	gedama_anim_0->addFrame(0, gedama_frame_00, 15);
	gedama_anim_0->addFrame(1, gedama_frame_01, 30);
	gedama_anim_0->addFrame(2, gedama_frame_02, 45);
	gedama_anim_0->addFrame(3, gedama_frame_03, 60);

	// Injure animation for Gedama
	descAnim.frameCount = 3;
	descAnim.isAnimLoop = FALSE;
	descAnim.next = 0;
	descAnim.state = 0;
	descAnim.timeToLive = -1;
	AEAnimation* gedama_anim_1 = new AEAnimation(descAnim);
	gedama_anim_1->addFrame(0, gedama_frame_10, 5);
	gedama_anim_1->addFrame(1, gedama_frame_11, 10);
	gedama_anim_1->addFrame(2, gedama_frame_12, 100);

	// Create Gedama object
	AERO_OBJECT_DESC descObj;
	descObj.oid = 0;
	descObj.name = "Gedama";
	descObj.otype = OBJ_CHARACTER;
	AEObject* gedama_obj = new AEObject(descObj);
	gedama_obj->addAnim(0, gedama_anim_0);
	gedama_obj->addAnim(1, gedama_anim_1);
	ae_ObjectTable.addAt(descObj.oid, gedama_obj);

	// Create BG layer animation. Only one frame (a static image) in this scene
	AEBGLayerFrame* bgLayerFrame = new AEBGLayerFrame(ae_ResourceTable.getItem(1), 0);
	AERO_BGLAYERANIM_DESC descBGLayerAnim;
	descBGLayerAnim.frameCount = 1;
	AEBGLayerAnim* bgLayerAnim = new AEBGLayerAnim(descBGLayerAnim);
	bgLayerAnim->addFrame(0, bgLayerFrame, 1000);

	// Create BG
	AERO_BACKGROUND_DESC descBG;
	descBG.name = "Starsky";
	AEBackground* bg_starsky = new AEBackground(descBG);

	// Add BG layer animation to BG
	bg_starsky->addAnimAt(0, bgLayerAnim);

	// Create BG layer
	AERO_BGLAYER_DESC descBGLayer;
	descBGLayer.depth = 0;
	descBGLayer.locX = -500.0f;
	descBGLayer.locY = -385.5f;
	descBGLayer.width = 1000;
	descBGLayer.height = 771;
	AEBGLayer* bgLayer = new AEBGLayer(descBGLayer);

	// Add BG layer anim reference to layer
	AEBGAnimRef* animRef = new AEBGAnimRef(0, 0, 0);
	bgLayer->addAnimRef(animRef);

	// Add BG layers to BG
	bg_starsky->addLayer(bgLayer);

	// Add BG to BG library
	ae_BGLibrary.add(bg_starsky);

	// Apply background
	setBackground(bg_starsky);

}

VOID GeneralSampleScene::initialize() {

	// Create a sample sprite
	AERO_SPRITE_DESC descSpr;
	descSpr.obj = ae_ObjectTable.getItem(0);
	descSpr.team = 0;
	descSpr.action = 0;
	descSpr.flip = SpriteEffects_None;
	descSpr.cx = 0.0f;
	descSpr.cy = 0.0f;
	AESprite* spr_gedama = new AESprite(descSpr);
	addSprite(spr_gedama);

}

VOID GeneralSampleScene::update() {
	AEScene::update();
}

VOID GeneralSampleScene::render() {
	AEScene::render();
}

VOID GeneralSampleScene::processInput() {

	if (keyStateBuffer[DIK_2] & 0x80) {
		ae_SceneManager.runScene(1);
	}
	if (keyStateBuffer[DIK_3] & 0x80) {
		ae_SceneManager.runScene(2);
	}

	if (keyStateBuffer[DIK_A] & 0x80) {
		AESprite* pGedama = ae_SceneManager.getActiveScene()->getSpriteTable()->getItemByHash(0);
		pGedama->adjustAlpha(-0.005f);
	}
	if (keyStateBuffer[DIK_S] & 0x80) {
		AESprite* pGedama = ae_SceneManager.getActiveScene()->getSpriteTable()->getItemByHash(0);
		pGedama->adjustAlpha(0.005f);
	}
	if (keyStateBuffer[DIK_Q] & 0x80) {
		AESprite* pGedama = ae_SceneManager.getActiveScene()->getSpriteTable()->getItemByHash(0);
		pGedama->rotateDeg(-1.0);
	}
	if (keyStateBuffer[DIK_W] & 0x80) {
		AESprite* pGedama = ae_SceneManager.getActiveScene()->getSpriteTable()->getItemByHash(0);
		pGedama->rotateDeg(1.0);
	}
	if (keyStateBuffer[DIK_E] & 0x80) {
		AESprite* pGedama = ae_SceneManager.getActiveScene()->getSpriteTable()->getItemByHash(0);
		pGedama->turnOverHorizontally();
	}

	if (keyStateBuffer[DIK_L] & 0x80) {
		int a = 1;
	}

}


VerticalScrollerScene::VerticalScrollerScene(AEBackground* _bg, AEHashedTable<AEPlatform>* _platformTable, AEHashedTable<AESprite>* _spriteTable, AEHeadUpDisplay* _hud) : AEScene(_bg, _platformTable, _spriteTable, _hud) {

	// Frames for JFighter
	AERO_FRAME_DESC descFrame;
	descFrame.res = ae_ResourceTable.getItem(2);
	descFrame.centerx = 25;
	descFrame.centery = 25;
	descFrame.dvx = 0;
	descFrame.dvy = 0;
	descFrame.imgOffset = 0;
	descFrame.imgCells = 1;
	descFrame.shiftx = 0;
	descFrame.shifty = 0;
	AEFrame* jf_frame_00 = new AEFrame(descFrame);
	descFrame.imgOffset = 1;
	AEFrame* jf_frame_01 = new AEFrame(descFrame);
	descFrame.imgOffset = 2;
	AEFrame* jf_frame_02 = new AEFrame(descFrame);

	// Frames for Rocket
	descFrame.res = ae_ResourceTable.getItem(3);
	descFrame.centerx = 8;
	descFrame.centery = 8;
	descFrame.dvx = 0;
	descFrame.dvy = 0;
	descFrame.imgOffset = 0;
	descFrame.imgCells = 1;
	descFrame.shiftx = 0;
	descFrame.shifty = 0;
	AEFrame* rocket_frame_00 = new AEFrame(descFrame);

	// Frames for Homing Bullet
	descFrame.imgOffset = 1;
	descFrame.imgCells = 1;
	AEFrame* homing_frame_00 = new AEFrame(descFrame);

	// Frames for Smoke
	descFrame.res = ae_ResourceTable.getItem(4);
	descFrame.centerx = 10;
	descFrame.centery = 10;
	descFrame.dvx = 0;
	descFrame.dvy = 0;
	descFrame.imgOffset = 0;
	descFrame.imgCells = 1;
	descFrame.shiftx = 0;
	descFrame.shifty = 0;
	AEFrame* smoke_frame_00 = new AEFrame(descFrame);
	descFrame.imgOffset = 1;
	AEFrame* smoke_frame_01 = new AEFrame(descFrame);
	descFrame.imgOffset = 2;
	AEFrame* smoke_frame_02 = new AEFrame(descFrame);
	descFrame.imgOffset = 3;
	AEFrame* smoke_frame_03 = new AEFrame(descFrame);
	descFrame.imgOffset = 4;
	AEFrame* smoke_frame_04 = new AEFrame(descFrame);
	descFrame.imgOffset = 5;
	AEFrame* smoke_frame_05 = new AEFrame(descFrame);
	descFrame.imgOffset = 6;
	AEFrame* smoke_frame_06 = new AEFrame(descFrame);
	descFrame.imgOffset = 7;
	AEFrame* smoke_frame_07 = new AEFrame(descFrame);

	// Frames for TurretBase
	descFrame.res = ae_ResourceTable.getItem(5);
	descFrame.centerx = 31;
	descFrame.centery = 31;
	descFrame.dvx = 0;
	descFrame.dvy = 0;
	descFrame.imgOffset = 0;
	descFrame.imgCells = 1;
	descFrame.shiftx = 0;
	descFrame.shifty = 0;
	AEFrame* turretbase_frame_00 = new AEFrame(descFrame);

	// Frames for Turret
	descFrame.imgOffset = 1;
	AEFrame* turret_frame_00 = new AEFrame(descFrame);

	// Animation for JFighter
	AERO_ANIMATION_DESC descAnim;
	descAnim.frameCount = 3;
	descAnim.isAnimLoop = TRUE;
	descAnim.next = AESprite::ACTION_NUM_DEAD;
	descAnim.state = 0;
	descAnim.timeToLive = -1;
	AEAnimation* jf_anim_0 = new AEAnimation(descAnim);
	jf_anim_0->addFrame(0, jf_frame_00, 5);
	jf_anim_0->addFrame(1, jf_frame_01, 10);
	jf_anim_0->addFrame(2, jf_frame_02, 15);

	// Animation for Rocket
	descAnim.frameCount = 1;
	descAnim.isAnimLoop = TRUE;
	descAnim.next = AESprite::ACTION_NUM_DEAD;
	descAnim.state = 0;
	descAnim.timeToLive = -1;
	AEAnimation* rocket_anim_0 = new AEAnimation(descAnim);
	rocket_anim_0->addFrame(0, rocket_frame_00, 100);

	// Animation for Homing Bullet
	descAnim.frameCount = 1;
	descAnim.isAnimLoop = TRUE;
	descAnim.next = AESprite::ACTION_NUM_DEAD;
	descAnim.state = 0;
	descAnim.timeToLive = -1;
	AEAnimation* homing_anim_0 = new AEAnimation(descAnim);
	homing_anim_0->addFrame(0, homing_frame_00, 100);

	// Animation for Smoke
	descAnim.frameCount = 8;
	descAnim.isAnimLoop = FALSE;
	descAnim.next = AESprite::ACTION_NUM_DEAD;
	descAnim.state = 0;
	descAnim.timeToLive = -1;
	AEAnimation* smoke_anim_0 = new AEAnimation(descAnim);
	smoke_anim_0->addFrame(0, smoke_frame_00, 3);
	smoke_anim_0->addFrame(1, smoke_frame_01, 6);
	smoke_anim_0->addFrame(2, smoke_frame_02, 9);
	smoke_anim_0->addFrame(3, smoke_frame_03, 12);
	smoke_anim_0->addFrame(4, smoke_frame_04, 15);
	smoke_anim_0->addFrame(5, smoke_frame_05, 18);
	smoke_anim_0->addFrame(6, smoke_frame_06, 21);
	smoke_anim_0->addFrame(7, smoke_frame_07, 24);

	// Animation for TurretBase
	descAnim.frameCount = 1;
	descAnim.isAnimLoop = TRUE;
	descAnim.next = AESprite::ACTION_NUM_DEAD;
	descAnim.state = 0;
	descAnim.timeToLive = -1;
	AEAnimation* turretbase_anim_0 = new AEAnimation(descAnim);
	turretbase_anim_0->addFrame(0, turretbase_frame_00, 100);

	// Animation for Turret Not Firing
	descAnim.frameCount = 1;
	descAnim.isAnimLoop = TRUE;
	descAnim.next = AESprite::ACTION_NUM_DEAD;
	descAnim.state = 0;
	descAnim.timeToLive = -1;
	AEAnimation* turret_anim_0 = new AEAnimation(descAnim);
	turret_anim_0->addFrame(0, turret_frame_00, 100);

	// Animation for Turret Firing
	descAnim.frameCount = 1;
	descAnim.isAnimLoop = FALSE;
	descAnim.next = 2;
	descAnim.state = 0;
	descAnim.timeToLive = -1;
	AEAnimation* turret_anim_1 = new AEAnimation(descAnim);
	turret_anim_1->addFrame(0, turret_frame_00, 120);

	// Animation for Turret Fire Cooldown
	descAnim.frameCount = 1;
	descAnim.isAnimLoop = FALSE;
	descAnim.next = 0;
	descAnim.state = 0;
	descAnim.timeToLive = -1;
	AEAnimation* turret_anim_2 = new AEAnimation(descAnim);
	turret_anim_2->addFrame(0, turret_frame_00, 60);

	// Create JFighter object
	AERO_OBJECT_DESC descObj;
	descObj.oid = 1;
	descObj.name = "Japanese Fighter";
	descObj.otype = OBJ_CHARACTER;
	AEObject* jf_obj = new AEObject(descObj);
	jf_obj->addAnim(0, jf_anim_0);
	ae_ObjectTable.addAt(descObj.oid, jf_obj);

	// Create Rocket object
	descObj.oid = 2;
	descObj.name = "Rocket";
	descObj.otype = OBJ_PROJECTILE;
	AEObject* rocket_obj = new AEObject(descObj);
	rocket_obj->addAnim(0, rocket_anim_0);
	ae_ObjectTable.addAt(descObj.oid, rocket_obj);

	// Create Homing Bullet object
	descObj.oid = 6;
	descObj.name = "Homing Bullet";
	descObj.otype = OBJ_PROJECTILE;
	AEObject* homing_obj = new AEObject(descObj);
	homing_obj->addAnim(0, homing_anim_0);
	ae_ObjectTable.addAt(descObj.oid, homing_obj);

	// Create Smoke object
	descObj.oid = 3;
	descObj.name = "Rocket Smoke";
	descObj.otype = OBJ_EFFECT;
	AEObject* smoke_obj = new AEObject(descObj);
	smoke_obj->addAnim(0, smoke_anim_0);
	ae_ObjectTable.addAt(descObj.oid, smoke_obj);

	// Create TurretBase object
	descObj.oid = 4;
	descObj.name = "Turret Base";
	descObj.otype = OBJ_CHARACTER;
	AEObject* turretbase_obj = new AEObject(descObj);
	turretbase_obj->addAnim(0, turretbase_anim_0);
	ae_ObjectTable.addAt(descObj.oid, turretbase_obj);

	// Create Turret object
	descObj.oid = 5;
	descObj.name = "Turret";
	descObj.otype = OBJ_CHARACTER;
	AEObject* turret_obj = new AEObject(descObj);
	turret_obj->addAnim(0, turret_anim_0);
	turret_obj->addAnim(1, turret_anim_1);
	turret_obj->addAnim(2, turret_anim_2);
	ae_ObjectTable.addAt(descObj.oid, turret_obj);

}

VerticalScrollerScene::~VerticalScrollerScene() {
	delete player;
}

VOID VerticalScrollerScene::initialize() {

	// Create Japanese Fighter
	AERO_SPRITE_DESC descSpr;
	descSpr.obj = ae_ObjectTable.getItem(1);
	descSpr.team = 0;
	descSpr.action = 0;
	descSpr.flip = SpriteEffects_None;
	descSpr.cx = 0.0f;
	descSpr.cy = 150.0f;
	descSpr.layerDepth = 0.0f;
	AESprite* jfighter = new JFighterSprite(descSpr);
	addSprite(jfighter);

	// Create a Turret Base
	descSpr.obj = ae_ObjectTable.getItem(4);
	descSpr.team = 1;
	descSpr.action = 0;
	descSpr.flip = SpriteEffects_None;
	descSpr.cx = 0.0f;
	descSpr.cy = -150.0f;
	descSpr.layerDepth = 1.0f;
	AESprite* turretbase = new AESprite(descSpr);
	addSprite(turretbase);
	turretbase->setAI(new TurretBaseAI(turretbase));

	// Create a Turret
	turretbase->createAttachmentTable(1);
	descSpr.obj = ae_ObjectTable.getItem(5);
	descSpr.team = 1;
	descSpr.action = 0;
	descSpr.flip = SpriteEffects_None;
	descSpr.cx = turretbase->getCx();
	descSpr.cy = turretbase->getCy();
	descSpr.layerDepth = 0.99f;
	Turret* spr_turret = new Turret(descSpr);
	spr_turret->setAI(new TurretAI(spr_turret));

	// Attach Turret to its Base
	turretbase->addAttachment(0, spr_turret);

	// Set player
	player = (JFighterSprite*)jfighter;
	player->rotateDeg(-90.0f);

}

VOID VerticalScrollerScene::update() {
	clipOutFarSprites();
	restrictPlayerScope();
	AEScene::update();
}

VOID VerticalScrollerScene::render() {
	AEScene::render();
}

VOID VerticalScrollerScene::processInput() {
	
	if (keyStateBuffer[DIK_1] & 0x80) {
		ae_SceneManager.runScene(0);
	}
	if (keyStateBuffer[DIK_3] & 0x80) {
		ae_SceneManager.runScene(2);
	}

	if (keyStateBuffer[DIK_A] & 0x80) {
		player->move(-1.6f, 0.0f);
	}
	if (keyStateBuffer[DIK_D] & 0x80) {
		player->move(1.6f, 0.0f);
	}
	if (keyStateBuffer[DIK_W] & 0x80) {
		player->move(0.0f, -1.2f);
	}
	if (keyStateBuffer[DIK_S] & 0x80) {
		player->move(0.0f, 1.2f);
	}
	if (keyStateBuffer[DIK_J] & 0x80) {
		if (!player->isFireKeyPressed) {
			player->isFireKeyPressed = TRUE;
			player->shoot();
		}
	}
	else {
		player->isFireKeyPressed = FALSE;
	}

	if (keyStateBuffer[DIK_L] & 0x80) {
		int a = 1;
	}

}

VOID VerticalScrollerScene::clipOutFarSprites() {
	for (INT iHash = 0; iHash < spriteTable->getHashCount(); iHash++) {
		AESprite* sprite = spriteTable->getItemByHash(iHash);
		AEPoint center = sprite->getCenter();
		if (center.x < (FLOAT)borderLeft || center.x >(FLOAT)borderRight || center.y < (FLOAT)borderTop || center.y > (FLOAT)borderBottom) {
			sprite->kill();
		}
	}
}

VOID VerticalScrollerScene::restrictPlayerScope() {
	AEPoint playerCenter = player->getCenter();
	if (playerCenter.x < (FLOAT)borderLeft + 100.0f) {
		player->setCx((FLOAT)borderLeft + 100.0f);
	}
	if (playerCenter.x > (FLOAT)borderRight - 100.0f) {
		player->setCx((FLOAT)borderRight - 100.0f);
	}
	if (playerCenter.y < (FLOAT)borderTop + 25.0f) {
		player->setCy((FLOAT)borderTop + 25.0f);
	}
	if (playerCenter.y > (FLOAT)borderBottom - 25.0f) {
		player->setCy((FLOAT)borderBottom - 25.0f);
	}
}


SideScrollerPlatformScene::SideScrollerPlatformScene(AEBackground* _bg, AEHashedTable<AEPlatform>* _platformTable, AEHashedTable<AESprite>* _spriteTable, AEHeadUpDisplay* _hud) : AEScene(_bg, _platformTable, _spriteTable, _hud) {
	
	// Frames for Helicopter
	AERO_FRAME_DESC descFrame;
	descFrame.res = ae_ResourceTable.getItem(6);
	descFrame.centerx = 29;
	descFrame.centery = 11;
	descFrame.dvx = 0;
	descFrame.dvy = 0;
	descFrame.imgOffset = 0;
	descFrame.imgCells = 1;
	descFrame.shiftx = 0;
	descFrame.shifty = 0;
	AEFrame* heli_frame_00 = new AEFrame(descFrame);
	descFrame.imgOffset = 1;
	AEFrame* heli_frame_01 = new AEFrame(descFrame);
	descFrame.imgOffset = 2;
	AEFrame* heli_frame_02 = new AEFrame(descFrame);

	// Frames for Flak Base
	descFrame.res = ae_ResourceTable.getItem(8);
	descFrame.centerx = 25;
	descFrame.centery = 27;
	descFrame.dvx = 0;
	descFrame.dvy = 0;
	descFrame.imgOffset = 0;
	descFrame.imgCells = 1;
	descFrame.shiftx = 0;
	descFrame.shifty = 0;
	AEFrame* flakbase_frame_00 = new AEFrame(descFrame);
	
	// Frames for Flak Cannon
	descFrame.centerx = 25;
	descFrame.centery = 10;
	descFrame.imgOffset = 1;
	AEFrame* flakcannon_frame_00 = new AEFrame(descFrame);
	descFrame.imgOffset = 2;
	AEFrame* flakcannon_frame_01 = new AEFrame(descFrame);
	descFrame.imgOffset = 3;
	AEFrame* flakcannon_frame_02 = new AEFrame(descFrame);
	descFrame.imgOffset = 4;
	AEFrame* flakcannon_frame_03 = new AEFrame(descFrame);

	// Frames for Flak Cannon Muzzle Flash
	descFrame.res = ae_ResourceTable.getItem(9);
	descFrame.centerx = 8;
	descFrame.centery = 13;
	descFrame.dvx = 0;
	descFrame.dvy = 0;
	descFrame.imgOffset = 0;
	descFrame.imgCells = 1;
	descFrame.shiftx = 0;
	descFrame.shifty = 0;
	AEFrame* flakflash_frame_00 = new AEFrame(descFrame);
	descFrame.imgOffset = 1;
	AEFrame* flakflash_frame_01 = new AEFrame(descFrame);

	// Frames for Shells
	descFrame.res = ae_ResourceTable.getItem(3);
	descFrame.centerx = 8;
	descFrame.centery = 8;
	descFrame.dvx = 0;
	descFrame.dvy = 0;
	descFrame.imgOffset = 2;
	descFrame.imgCells = 1;
	descFrame.shiftx = 0;
	descFrame.shifty = 0;
	AEFrame* shell_frame_00 = new AEFrame(descFrame);

	// Animation for Helicopter
	AERO_ANIMATION_DESC descAnim;
	descAnim.frameCount = 3;
	descAnim.isAnimLoop = TRUE;
	descAnim.next = AESprite::ACTION_NUM_DEAD;
	descAnim.state = 0;
	descAnim.timeToLive = -1;
	AEAnimation* heli_anim_0 = new AEAnimation(descAnim);
	heli_anim_0->addFrame(0, heli_frame_00, 5);
	heli_anim_0->addFrame(1, heli_frame_01, 10);
	heli_anim_0->addFrame(2, heli_frame_02, 15);

	// Animation for Flak Base
	descAnim.frameCount = 1;
	descAnim.isAnimLoop = TRUE;
	descAnim.next = AESprite::ACTION_NUM_DEAD;
	descAnim.state = 0;
	descAnim.timeToLive = -1;
	AEAnimation* flakbase_anim_0 = new AEAnimation(descAnim);
	flakbase_anim_0->addFrame(0, flakbase_frame_00, 100);

	// Animation for Flak Cannon Not Firing
	descAnim.frameCount = 1;
	descAnim.isAnimLoop = TRUE;
	descAnim.next = AESprite::ACTION_NUM_DEAD;
	descAnim.state = 0;
	descAnim.timeToLive = -1;
	AEAnimation* flakcannon_anim_0 = new AEAnimation(descAnim);
	flakcannon_anim_0->addFrame(0, flakcannon_frame_00, 100);

	// Animation for Flak Cannon Firing
	descAnim.frameCount = 4;
	descAnim.isAnimLoop = TRUE;
	descAnim.next = 2;
	descAnim.state = 0;
	descAnim.timeToLive = 120;
	AEAnimation* flakcannon_anim_1 = new AEAnimation(descAnim);
	flakcannon_anim_1->addFrame(0, flakcannon_frame_00, 3);
	flakcannon_anim_1->addFrame(1, flakcannon_frame_01, 6);
	flakcannon_anim_1->addFrame(2, flakcannon_frame_02, 9);
	flakcannon_anim_1->addFrame(3, flakcannon_frame_03, 12);

	// Animation for Flak Cannon Fire Cooldown
	descAnim.frameCount = 1;
	descAnim.isAnimLoop = FALSE;
	descAnim.next = 0;
	descAnim.state = 0;
	descAnim.timeToLive = -1;
	AEAnimation* flakcannon_anim_2 = new AEAnimation(descAnim);
	flakcannon_anim_2->addFrame(0, flakcannon_frame_00, 60);

	// Animation for Flak Cannon Muzzle Flash
	descAnim.frameCount = 2;
	descAnim.isAnimLoop = FALSE;
	descAnim.next = AESprite::ACTION_NUM_DEAD;
	descAnim.state = 0;
	descAnim.timeToLive = -1;
	AEAnimation* flakflash_anim_0 = new AEAnimation(descAnim);
	flakflash_anim_0->addFrame(0, flakflash_frame_00, 2);
	flakflash_anim_0->addFrame(1, flakflash_frame_01, 4);

	// Animation for Shells
	descAnim.frameCount = 1;
	descAnim.isAnimLoop = TRUE;
	descAnim.next = AESprite::ACTION_NUM_DEAD;
	descAnim.state = 0;
	descAnim.timeToLive = 300;
	AEAnimation* shell_anim_0 = new AEAnimation(descAnim);
	shell_anim_0->addFrame(0, shell_frame_00, 100);

	// Create Helicopter object
	AERO_OBJECT_DESC descObj;
	descObj.oid = 7;
	descObj.name = "Helicopter";
	descObj.otype = OBJ_CHARACTER;
	AEObject* heli_obj = new AEObject(descObj);
	heli_obj->addAnim(0, heli_anim_0);
	ae_ObjectTable.addAt(descObj.oid, heli_obj);

	// Create Flak Base object
	descObj.oid = 8;
	descObj.name = "Flak Base";
	descObj.otype = OBJ_CHARACTER;
	AEObject* flakbase_obj = new AEObject(descObj);
	flakbase_obj->addAnim(0, flakbase_anim_0);
	ae_ObjectTable.addAt(descObj.oid, flakbase_obj);

	// Create Flak Cannon object
	descObj.oid = 9;
	descObj.name = "Flak Cannon";
	descObj.otype = OBJ_CHARACTER;
	AEObject* flakcannon_obj = new AEObject(descObj);
	flakcannon_obj->addAnim(0, flakcannon_anim_0);
	flakcannon_obj->addAnim(1, flakcannon_anim_1);
	flakcannon_obj->addAnim(2, flakcannon_anim_2);
	ae_ObjectTable.addAt(descObj.oid, flakcannon_obj);

	// Create Flak Cannon Muzzle Flash object
	descObj.oid = 10;
	descObj.name = "Flak Muzzle Flash";
	descObj.otype = OBJ_EFFECT;
	AEObject* flakflash_obj = new AEObject(descObj);
	flakflash_obj->addAnim(0, flakflash_anim_0);
	ae_ObjectTable.addAt(descObj.oid, flakflash_obj);

	// Create Shell object
	descObj.oid = 11;
	descObj.name = "Bullet Shell";
	descObj.otype = OBJ_EFFECT;
	AEObject* shell_obj = new AEObject(descObj);
	shell_obj->addAnim(0, shell_anim_0);
	ae_ObjectTable.addAt(descObj.oid, shell_obj);

	// Create BG layer animation. Only one frame (a static image) in this scene
	AEBGLayerFrame* bgLayerFrame = new AEBGLayerFrame(ae_ResourceTable.getItem(7), 0);
	AERO_BGLAYERANIM_DESC descBGLayerAnim;
	descBGLayerAnim.frameCount = 1;
	AEBGLayerAnim* bgLayerAnim = new AEBGLayerAnim(descBGLayerAnim);
	bgLayerAnim->addFrame(0, bgLayerFrame, 1000);

	// Create BG
	AERO_BACKGROUND_DESC descBG;
	descBG.name = "Cave";
	AEBackground* bg_cave = new AEBackground(descBG);

	// Add BG layer animation to BG
	bg_cave->addAnimAt(0, bgLayerAnim);

	// Create BG layer
	AERO_BGLAYER_DESC descBGLayer;
	descBGLayer.depth = 0;
	descBGLayer.locX = -320.0f;
	descBGLayer.locY = -240.5f;
	descBGLayer.width = 640;
	descBGLayer.height = 480;
	AEBGLayer* bgLayer = new AEBGLayer(descBGLayer);

	// Add BG layer anim reference to layer
	AEBGAnimRef* animRef = new AEBGAnimRef(0, 0, 0);
	bgLayer->addAnimRef(animRef);

	// Add BG layers to BG
	bg_cave->addLayer(bgLayer);

	// Add BG to BG library
	ae_BGLibrary.add(bg_cave);

	// Apply background
	setBackground(bg_cave);

	// Create Platforms
	AEPlatform* platform_floor = new AEPlatform(9);
	platform_floor->addNode({ -320.0f, 183.0f });
	platform_floor->addNode({ -87.0f, 183.0f });
	platform_floor->addNode({ 1.0f, 156.0f });
	platform_floor->addNode({ 99.0f, 83.0f });
	platform_floor->addNode({ 216.0f, 115.0f });
	platform_floor->addNode({ 272.0f, 90.0f });
	platform_floor->addNode({ 320.0f, 6.0f });
	platform_floor->addNode({ 320.0f, -165.0f });
	platform_floor->addNode({ -56.0f, -240.0f });

	AEPlatform* platform_rock = new AEPlatform(6);
	platform_rock->addNode({ 26.0f, -30.0f });
	platform_rock->addNode({ 107.0f, -135.0f });
	platform_rock->addNode({ 164.0f, -119.0f });
	platform_rock->addNode({ 175.0f, -67.0f });
	platform_rock->addNode({ 124.0f, -6.0f });
	platform_rock->addNode({ 26.0f, -30.0f });

	// Apply platforms
	AEHashedTable<AEPlatform>* platforms_cave = new AEHashedTable<AEPlatform>(10);
	platforms_cave->add(platform_floor);
	platforms_cave->add(platform_rock);
	setPlatformTable(platforms_cave);

}

SideScrollerPlatformScene::~SideScrollerPlatformScene() {
	delete player;
}

VOID SideScrollerPlatformScene::initialize() {

	// Create Helicopter
	AERO_SPRITE_DESC descSpr;
	descSpr.obj = ae_ObjectTable.getItem(7);
	descSpr.team = 0;
	descSpr.action = 0;
	descSpr.flip = SpriteEffects_None;
	descSpr.cx = -200.0f;
	descSpr.cy = -100.0f;
	descSpr.layerDepth = 0.0f;
	Helicopter* heli = new Helicopter(descSpr);
	addSprite(heli);

	// Create Flak Base
	descSpr.obj = ae_ObjectTable.getItem(8);
	descSpr.team = 1;
	descSpr.action = 0;
	descSpr.flip = SpriteEffects_FlipHorizontally;
	descSpr.cx = 105.0f;
	descSpr.cy = 87.0f;
	descSpr.layerDepth = 0.1f;
	AESprite* flakBase = new AESprite(descSpr);
	addSprite(flakBase);

	// Create Flak Cannon
	descSpr.obj = ae_ObjectTable.getItem(9);
	descSpr.team = 1;
	descSpr.action = 0;
	descSpr.flip = SpriteEffects_FlipHorizontally;
	descSpr.cx = 105.0f;
	descSpr.cy = 70.0f;
	descSpr.layerDepth = 0.2f;
	AESprite* flakCannon = new AESprite(descSpr);
	flakCannon->setAI(new FlakCannonAI(flakCannon));

	// Attach Flak Cannon to Flak Base
	flakBase->createAttachmentTable(1);
	flakBase->addAttachment(0, flakCannon);

	// Set player
	player = (Helicopter*)heli;

}

VOID SideScrollerPlatformScene::update() {
	AEScene::update();
}

VOID SideScrollerPlatformScene::render() {
	AEScene::render();
}

VOID SideScrollerPlatformScene::processInput() {

	FLOAT lift = 0.0f, thrust = 0.0f, weight = 0.0f, drag = 0.0f;
	FLOAT pitch_down_hor = 0.0f, pitch_up_hor = 0.0f, pitch_down_ver = 0.0f, pitch_up_ver = 0.0f;
	if (keyStateBuffer[DIK_1] & 0x80) {
		ae_SceneManager.runScene(0);
	}
	if (keyStateBuffer[DIK_2] & 0x80) {
		ae_SceneManager.runScene(1);
	}

	if (keyStateBuffer[DIK_A] & 0x80) {
		drag = 1.0f;
		pitch_up_hor = 0.8f;
	}
	else {
		drag = 0.0f;
		pitch_up_hor = 0.0f;
	}
	if (keyStateBuffer[DIK_D] & 0x80) {
		thrust = 1.5f;
		pitch_down_hor = 1.0f;
	}
	else {
		thrust = 0.0f;
		pitch_down_hor = 0.0f;
	}
	if (keyStateBuffer[DIK_W] & 0x80) {
		lift = 0.8f;
		pitch_up_ver = 0.6f;
	}
	else {
		lift = 0.0f;
		pitch_up_ver = 0.0f;
	}
	if (keyStateBuffer[DIK_S] & 0x80) {
		weight = 1.2f;
		pitch_down_ver = 1.0f;
	}
	else {
		weight = 0.0f;
		pitch_down_ver = 0.0f;
	}
	player->setVx(thrust - drag);
	player->setVy(weight - lift);
	player->setVAngleDeg(pitch_down_hor + pitch_down_ver - pitch_up_hor - pitch_up_ver, AESprite::ANGLE_DISPLAY);

	if (keyStateBuffer[DIK_J] & 0x80) {
		if (!player->isFireKeyPressed) {
			player->isFireKeyPressed = TRUE;
			
		}
	}
	else {
		player->isFireKeyPressed = FALSE;
	}

	if (keyStateBuffer[DIK_L] & 0x80) {
		int a = 1;
	}

}