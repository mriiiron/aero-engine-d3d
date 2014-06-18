#include <dinput.h>

#include "AEroEngine.h"
#include "Sprites.h"
#include "Scenes.h"

extern AEResourceTable						ae_ResourceTable;
extern AEConstantTable<AEObject>			ae_ObjectTable;
extern AEBackgroundLibrary					ae_BGLibrary;
extern AESceneManager						ae_SceneManager;

GeneralSampleScene::GeneralSampleScene(AEBackground* _bg, AEHashedTable<AESprite>* _spriteTable, AEHeadUpDisplay* _hud) : AEScene(_bg, _spriteTable, _hud) {
	
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
	descAnim.next = 0;
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
	ae_ObjectTable.addAt(0, gedama_obj);

	// Create BG layer animation. Only one frame (a static image) in this scene
	AEBGLayerFrame* bgLayerFrame = new AEBGLayerFrame(ae_ResourceTable.getItem(1), 0);
	AERO_BGLAYERANIM_DESC descBGLayerAnim;
	descBGLayerAnim.frameCount = 1;
	AEBGLayerAnim* bgLayerAnim = new AEBGLayerAnim(descBGLayerAnim);
	bgLayerAnim->addFrame(0, bgLayerFrame, 1000);

	// Create BG
	AERO_BACKGROUND_DESC descBG;
	descBG.name = "Starsky";
	AEBackground* bg = new AEBackground(descBG);

	// Add BG layer animation to BG
	bg->addAnimAt(0, bgLayerAnim);

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
	bg->addLayer(bgLayer);

	// Add BG to BG library
	ae_BGLibrary.add(bg);

}

VOID GeneralSampleScene::initialize() {

	// Create a sample sprite
	AERO_SPRITE_DESC descSpr;
	descSpr.obj = ae_ObjectTable.getItem(0);
	descSpr.team = 0;
	descSpr.action = 0;
	descSpr.facing = SpriteEffects_None;
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
		pGedama->rotateDeg(1.0);
	}
	if (keyStateBuffer[DIK_W] & 0x80) {
		AESprite* pGedama = ae_SceneManager.getActiveScene()->getSpriteTable()->getItemByHash(0);
		pGedama->rotateDeg(-1.0);
	}

	if (keyStateBuffer[DIK_L] & 0x80) {
		textDebug = textDebug;
	}

}


VerticalScrollerScene::VerticalScrollerScene(AEBackground* _bg, AEHashedTable<AESprite>* _spriteTable, AEHeadUpDisplay* _hud) : AEScene(_bg, _spriteTable, _hud) {

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
	descFrame.centerx = 4;
	descFrame.centery = 9;
	descFrame.dvx = 0;
	descFrame.dvy = 0;
	descFrame.imgOffset = 0;
	descFrame.imgCells = 1;
	descFrame.shiftx = 0;
	descFrame.shifty = 0;
	AEFrame* rocket_frame_00 = new AEFrame(descFrame);

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

	// Animation for JFighter
	AERO_ANIMATION_DESC descAnim;
	descAnim.frameCount = 3;
	descAnim.isAnimLoop = TRUE;
	descAnim.next = 0;
	descAnim.state = 0;
	descAnim.timeToLive = -1;
	AEAnimation* jf_anim_0 = new AEAnimation(descAnim);
	jf_anim_0->addFrame(0, jf_frame_00, 5);
	jf_anim_0->addFrame(1, jf_frame_01, 10);
	jf_anim_0->addFrame(2, jf_frame_02, 15);

	// Animation for Rocket
	descAnim.frameCount = 1;
	descAnim.isAnimLoop = TRUE;
	descAnim.next = 0;
	descAnim.state = 0;
	descAnim.timeToLive = -1;
	AEAnimation* rocket_anim_0 = new AEAnimation(descAnim);
	rocket_anim_0->addFrame(0, rocket_frame_00, 100);

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

	// Create JFighter object
	AERO_OBJECT_DESC descObj;
	descObj.oid = 1;
	descObj.name = "Japanese Fighter";
	descObj.otype = OBJ_CHARACTER;
	AEObject* jf_obj = new AEObject(descObj);
	jf_obj->addAnim(0, jf_anim_0);
	ae_ObjectTable.addAt(1, jf_obj);

	// Create Rocket object
	descObj.oid = 2;
	descObj.name = "Rocket";
	descObj.otype = OBJ_PROJECTILE;
	AEObject* rocket_obj = new AEObject(descObj);
	rocket_obj->addAnim(0, rocket_anim_0);
	ae_ObjectTable.addAt(2, rocket_obj);

	// Create Smoke object
	descObj.oid = 3;
	descObj.name = "Rocket Smoke";
	descObj.otype = OBJ_EFFECT;
	AEObject* smoke_obj = new AEObject(descObj);
	smoke_obj->addAnim(0, smoke_anim_0);
	ae_ObjectTable.addAt(3, smoke_obj);

}

VOID VerticalScrollerScene::initialize() {

	// Create player
	AERO_SPRITE_DESC descSpr;
	descSpr.obj = ae_ObjectTable.getItem(1);
	descSpr.team = 0;
	descSpr.action = 0;
	descSpr.facing = SpriteEffects_None;
	descSpr.cx = 0.0f;
	descSpr.cy = 150.0f;
	AESprite* spr_jfighter = new JFighterSprite(descSpr);
	addSprite(spr_jfighter);
	player = (JFighterSprite*)(spriteTable->getItemByHash(0));

}

VOID VerticalScrollerScene::update() {
	clipOutFarSprites();
	AEScene::update();
}

VOID VerticalScrollerScene::render() {
	AEScene::render();
}

VOID VerticalScrollerScene::processInput() {
	
	if (keyStateBuffer[DIK_1] & 0x80) {
		ae_SceneManager.runScene(0);
	}

	if (keyStateBuffer[DIK_A] & 0x80) {
		player->move(-0.7f, 0.0f);
	}
	if (keyStateBuffer[DIK_D] & 0x80) {
		player->move(0.7f, 0.0f);
	}
	if (keyStateBuffer[DIK_W] & 0x80) {
		player->move(0.0f, -0.7f);
	}
	if (keyStateBuffer[DIK_S] & 0x80) {
		player->move(0.0f, 0.7f);
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


	if (keyStateBuffer[DIK_K] & 0x80) {
		textDebug = TRUE;
	}
	else {
		textDebug = FALSE;
	}

	if (keyStateBuffer[DIK_L] & 0x80) {
		textDebug = textDebug;
	}

}

VOID VerticalScrollerScene::clipOutFarSprites() {
	for (INT iHash = 0; iHash < spriteTable->getHashCount(); iHash++) {
		AESprite* sprite = spriteTable->getItemByHash(iHash);
		AEPoint center = sprite->getCenter();
		if (center.x < -320.0f || center.x > 320.0f || center.y < -240.0f || center.y > 240.0f) {
			sprite->kill();
		}
	}
}