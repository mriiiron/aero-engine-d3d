#include <dinput.h>

#include "AEroEngine.h"
#include "WLFFileReader.h"
#include "WLFData.h"
#include "WLFAI.h"
#include "WLFAnimation.h"
#include "WLFBuff.h"
#include "WLFSprite.h"
#include "WLFHeadUpDisplay.h"
#include "WLFSpell.h"
#include "WLFScene.h"

extern AEConstantTable<AEResource>          ae_ResourceTable;
extern AEConstantTable<AEObject>            ae_ObjectTable;
extern AEBackgroundLibrary                  ae_BGLibrary;
extern AESceneManager                       ae_SceneManager;
extern AECamera                             ae_Camera;

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

    loadObject(0, "Resources\\warrior_deep.txt", "Warrior Deep", AEObject::OBJ_CHARACTER);
    loadObject(1, "Resources\\phantom_bandit.txt", "Phantom Bandit", AEObject::OBJ_CHARACTER);
    loadObject(2, "Resources\\dummy.txt", "Training Dummy", AEObject::OBJ_CHARACTER);
    loadObject(10, "Resources\\effects.txt", "Effect", AEObject::OBJ_EFFECT);
    loadObject(11, "Resources\\icons.txt", "Icon", AEObject::OBJ_EFFECT);
    loadObject(12, "Resources\\sparks.txt", "Hit Spark", AEObject::OBJ_HIT_SPARK);
    loadObject(13, "Resources\\bandit_weapon.txt", "Bandit Weapon Shatters", AEObject::OBJ_EFFECT);
    loadObject(40, "Resources\\ui.txt", "UI", AEObject::OBJ_EFFECT);

    // Create Shrine Cave BG

    AERO_BGLAYERANIM_DESC descBGLayerAnim;
    AEBGLayerFrame* bgLayerFrame_Cave = new AEBGLayerFrame(ae_ResourceTable.getItem(80), 0);
    descBGLayerAnim.frameCount = 1;
    AEBGLayerAnim* bgLayerAnim_Cave = new AEBGLayerAnim(descBGLayerAnim);
    bgLayerAnim_Cave->addFrame(0, bgLayerFrame_Cave, 1000);

    AEBGLayerFrame* bgLayerFrame_Roof = new AEBGLayerFrame(ae_ResourceTable.getItem(81), 0);
    descBGLayerAnim.frameCount = 1;
    AEBGLayerAnim* bgLayerAnim_Roof = new AEBGLayerAnim(descBGLayerAnim);
    bgLayerAnim_Roof->addFrame(0, bgLayerFrame_Roof, 1000);

    AERO_BACKGROUND_DESC descBG;
    descBG.name = "Shrine Cave";
    AEBackground* bg_shrine_cave = new AEBackground(descBG);
    bg_shrine_cave->addAnimAt(0, bgLayerAnim_Cave);
    bg_shrine_cave->addAnimAt(1, bgLayerAnim_Roof);

    AERO_BGLAYER_DESC descBGLayer;
    descBGLayer.depth = 0;
    descBGLayer.locX = -450.0f;
    descBGLayer.locY = -240.0f;
    descBGLayer.width = 900;
    descBGLayer.height = 480;
    AEBGLayer* caveLayer = new AEBGLayer(descBGLayer);
    AEBGAnimRef* animRef_Cave = new AEBGAnimRef(0, 0, 0);
    caveLayer->addAnimRef(animRef_Cave);

    descBGLayer.depth = -25;
    descBGLayer.locX = -465.0f;
    descBGLayer.locY = -240.0f;
    descBGLayer.width = 930;
    descBGLayer.height = 160;
    AEBGLayer* roofLayer = new AEBGLayer(descBGLayer);
    AEBGAnimRef* animRef_Roof = new AEBGAnimRef(0, 0, 1);
    roofLayer->addAnimRef(animRef_Roof);

    bg_shrine_cave->addLayer(roofLayer);
    bg_shrine_cave->addLayer(caveLayer);

    ae_BGLibrary.add(bg_shrine_cave);
    this->setBackground(bg_shrine_cave);

    // Create Platforms
    AEPlatform* platform_floor = new AEPlatform(14);
    platform_floor->addNode({ descBGLayer.locX + -321.0f, descBGLayer.locY + 352.0f });
    platform_floor->addNode({ descBGLayer.locX + -85.0f, descBGLayer.locY + 352.0f });
    platform_floor->addNode({ descBGLayer.locX + -34.0f, descBGLayer.locY + 313.0f });
    platform_floor->addNode({ descBGLayer.locX + 150.0f, descBGLayer.locY + 310.0f });
    platform_floor->addNode({ descBGLayer.locX + 231.0f, descBGLayer.locY + 368.0f });
    platform_floor->addNode({ descBGLayer.locX + 322.0f, descBGLayer.locY + 390.0f });
    platform_floor->addNode({ descBGLayer.locX + 546.0f, descBGLayer.locY + 390.0f });
    platform_floor->addNode({ descBGLayer.locX + 625.0f, descBGLayer.locY + 344.0f });
    platform_floor->addNode({ descBGLayer.locX + 669.0f, descBGLayer.locY + 344.0f });
    platform_floor->addNode({ descBGLayer.locX + 725.0f, descBGLayer.locY + 382.0f });
    platform_floor->addNode({ descBGLayer.locX + 837.0f, descBGLayer.locY + 382.0f });
    platform_floor->addNode({ descBGLayer.locX + 1049.0f, descBGLayer.locY + 318.0f });
    platform_floor->addNode({ descBGLayer.locX + 1112.0f, descBGLayer.locY + 278.0f });
    platform_floor->addNode({ descBGLayer.locX + 1179.0f, descBGLayer.locY + 264.0f });
    platform_floor->calcAngles();

    // Apply platforms
    AEHashedTable<AEPlatform>* platforms_shrine_cave = new AEHashedTable<AEPlatform>(10);
    platforms_shrine_cave->add(platform_floor);
    this->setPlatformTable(platforms_shrine_cave);

    // Add HUD
    hud = new WLFHeadUpDisplay(100);
    hud->setScene(this);

    standstill = 0;

}

VOID WLFShrineCaveScene::initialize() {

    // Create Player
    AERO_SPRITE_DESC descSpr;
    descSpr.obj = ae_ObjectTable.getItem(0);
    descSpr.team = 0;
    descSpr.action = WLFCharacter::ACTION_IN_AIR;
    descSpr.flip = SpriteEffects_None;
    descSpr.cx = -200.0f;
    descSpr.cy = 0.0f;
    descSpr.layerDepth = 0.001f;
    WLFWarrior* deep = new WLFWarrior(descSpr);
    addSprite(deep);

    // Set player
    player = deep;
    deep->setPlayerFlag(true);

    // Set camera focusing on player
    ae_Camera.setFocus(player->getCx(), 0.0f);

    // Create Bandit
    descSpr.obj = ae_ObjectTable.getItem(1);
    descSpr.team = 1;
    descSpr.action = WLFCharacter::ACTION_IN_AIR;
    descSpr.flip = SpriteEffects_FlipHorizontally;
    descSpr.cx = 0.0f;
    descSpr.cy = 0.0f;  // 150.0f;
    descSpr.layerDepth = 0.01f;
    WLFBandit* bandit = new WLFBandit(descSpr);
    bandit->setAI(new WLFBanditAI(bandit));
    bandit->setPortraitIndex(11);
    addSprite(bandit);

    // Create Dummy
    descSpr.obj = ae_ObjectTable.getItem(2);
    descSpr.cx = 210.0f;
    descSpr.cy = 0.0f;  // 150.0f;
    WLFCharacter* dummy = new WLFCharacter(descSpr);
    dummy->setPortraitIndex(12);
    addSprite(dummy);

    // Create player namepad
    //addNamepadToHUD(deep, 10, NAMEPAD_SLOT_PLAYER);
    dynamic_cast<WLFHeadUpDisplay*>(hud)->setNamepad(WLFHeadUpDisplay::NAMEPAD_SLOT_PLAYER, deep);

}

VOID WLFShrineCaveScene::loadObject(INT oid, std::string fileName, std::string objName, INT objType) {
    AERO_OBJECT_DESC descObj;
    descObj.oid = oid;
    descObj.name = objName;
    descObj.type = objType;
    AEObject* obj = new AEObject(descObj);
    WLFDataFileReader::readObject(fileName, obj);
    ae_ObjectTable.addAt(oid, obj);
}

VOID WLFShrineCaveScene::addNamepadToHUD(WLFCharacter* character, INT portraitIndex, INT slot) {

    FLOAT namepadPosX = -300.0f + 150.0f * slot;
    FLOAT namepadPosY = -220.0f;

    AERO_SPRITE_DESC descSpr;
    descSpr.obj = ae_ObjectTable.getItem(40);
    descSpr.team = -1;
    descSpr.action = 0;
    descSpr.flip = SpriteEffects_None;
    descSpr.cx = namepadPosX;
    descSpr.cy = namepadPosY;
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

// Override AEScene::update() completely
VOID WLFShrineCaveScene::update() {
    if (isPaused) {
        return;
    }
    if (standstill > 0) {
        standstill--;
    }
    processCollision();
    if (bg) {
        bg->update();
    }
    if (spriteTable) {
        for (INT iHash = 0; iHash < spriteTable->getHashCount(); iHash++) {
            AESprite* sprite = spriteTable->getItemByHash(iHash);
            if (sprite->isDead()) {
                spriteTable->removeItemByHash(iHash);
            }
            else {
                sprite->update(platformTable);
            }
        }
        for (INT iHash = 0; iHash < spriteTable->getHashCount(); iHash++) {
            AESprite* sprite = spriteTable->getItemByHash(iHash);
            if (sprite->hasAttachments()) {
                sprite->updateAttachments();
            }
        }
    }
    if (hud) {
        hud->update();
    }
    FLOAT focusX = player->getCx();
    if (focusX < -85.0f) focusX = -85.0f;
    if (focusX > 90.0f) focusX = 90.0f;
    ae_Camera.setFocus(focusX, 0.0f);
    //ae_Camera.setFocus(0.0f, 0.0f);
    ae_Camera.update();
}

VOID WLFShrineCaveScene::render(INT renderMode) {
    AEScene::render(renderMode);
}

VOID WLFShrineCaveScene::processInput() {

    if (!isPaused) {

        //
        // TODO: Processing Input
        //

        // Up Key (^)
        if (keyStateBuffer[dik_up] & 0x80) {
            player->keyDown(WLFKeys::KEY_UP);
        }
        else {
            player->keyUp(WLFKeys::KEY_UP);
        }

        // Down Key (v)
        if (keyStateBuffer[dik_down] & 0x80) {
            player->keyDown(WLFKeys::KEY_DOWN);
        }
        else {
            player->keyUp(WLFKeys::KEY_DOWN);
        }

        // Left Key (<)
        if (keyStateBuffer[dik_left] & 0x80) {
            player->keyDown(WLFKeys::KEY_LEFT);
        }
        else {
            player->keyUp(WLFKeys::KEY_LEFT);
        }

        // Right Key (>)
        if (keyStateBuffer[dik_right] & 0x80) {
            player->keyDown(WLFKeys::KEY_RIGHT);
        }
        else {
            player->keyUp(WLFKeys::KEY_RIGHT);
        }

        // A Key (A)
        if (keyStateBuffer[dik_attack_a] & 0x80) {
            if (!player->isKeyPressed(WLFKeys::KEY_ATTACK_A)) {
                if ((player->getFlip() == SpriteEffects::SpriteEffects_None && player->isKeyPressed(WLFKeys::KEY_RIGHT)) || (player->getFlip() == SpriteEffects::SpriteEffects_FlipHorizontally && player->isKeyPressed(WLFKeys::KEY_LEFT))) {
                    player->applyMoveInput(WLFCharacter::MOVE_FORWARD_A);
                    player->applyMoveInput(WLFCharacter::MOVE_A);
                }
                else if ((player->getFlip() == SpriteEffects::SpriteEffects_None && player->isKeyPressed(WLFKeys::KEY_RIGHT)) || (player->getFlip() == SpriteEffects::SpriteEffects_FlipHorizontally && player->isKeyPressed(WLFKeys::KEY_LEFT))) {
                    player->applyMoveInput(WLFCharacter::MOVE_DOWN_A);
                }
                else {
                    player->applyMoveInput(WLFCharacter::MOVE_A);
                }	
            }
            player->keyDown(WLFKeys::KEY_ATTACK_A);
        }
        else {
            player->keyUp(WLFKeys::KEY_ATTACK_A);
        }

        // B Key (B)
        if (keyStateBuffer[dik_attack_b] & 0x80) {
            if (!player->isKeyPressed(WLFKeys::KEY_ATTACK_B)) {
                if ((player->getFlip() == SpriteEffects::SpriteEffects_None && player->isKeyPressed(WLFKeys::KEY_RIGHT)) || (player->getFlip() == SpriteEffects::SpriteEffects_FlipHorizontally && player->isKeyPressed(WLFKeys::KEY_LEFT))) {
                    player->applyMoveInput(WLFCharacter::MOVE_FORWARD_B);
                    player->applyMoveInput(WLFCharacter::MOVE_B);
                }
                else if (player->isKeyPressed(WLFKeys::KEY_DOWN)) {
                    player->applyMoveInput(WLFCharacter::MOVE_DOWN_B);
                    player->applyMoveInput(WLFCharacter::MOVE_B);
                }
                else {
                    player->applyMoveInput(WLFCharacter::MOVE_B);
                }
            }
            player->keyDown(WLFKeys::KEY_ATTACK_B);
        }
        else {
            player->keyUp(WLFKeys::KEY_ATTACK_B);
        }

        if (keyStateBuffer[dik_jump] & 0x80) {
            if (!player->isKeyPressed(WLFKeys::KEY_JUMP) && player->getState() != WLFCharacter::STATE_IN_AIR) {
                player->applyMoveInput(WLFCharacter::MOVE_JUMP);
            }
            player->keyDown(WLFKeys::KEY_JUMP);
        }
        else {
            player->keyUp(WLFKeys::KEY_JUMP);
        }

        // Spetial Key (SP)
        if (keyStateBuffer[dik_special] & 0x80) {
            if (!player->isKeyPressed(WLFKeys::KEY_SPECIAL) && player->getTarget() != nullptr && (player->getAction() >= 0 || player->getAction() <= 5)) {
                // TODO: Special Key
            }
            player->keyDown(WLFKeys::KEY_SPECIAL);
        }
        else {
            player->keyUp(WLFKeys::KEY_SPECIAL);
        }

        if (keyStateBuffer[DIK_TAB] & 0x80) {
            if (!player->isKeyPressed(WLFKeys::KEY_CHANGE_TARGET)) {
                player->changeTarget();
            }
            player->keyDown(WLFKeys::KEY_CHANGE_TARGET);
        }
        else {
            player->keyUp(WLFKeys::KEY_CHANGE_TARGET);
        }

    }
    if (keyStateBuffer[DIK_Q] & 0x80) {
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
        if (sprite1->getObject() == nullptr) {
            continue;
        }
        WLFAnimation* anim1 = (WLFAnimation*)(sprite1->getCurrentAnimation());
        WLFAttackJudgeArea* attackJudge = anim1->getAttackJudge(sprite1->getFrameNum());
        if (attackJudge == nullptr || dynamic_cast<WLFCharacter*>(sprite1)->isAttackLocked()) {
            continue;
        }
        for (INT j = 0; j < spriteCount; j++) {
            AESprite* sprite2 = spriteTable->getItemByHash(j);
            if (i == j || sprite1->getTeam() == sprite2->getTeam() || sprite2->getObject() == nullptr) {
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

                if (sprite1->getFlip() != sprite2->getFlip()) {
                    sprite2->changeAction(AENSMath::randomIntBetween(WLFCharacter::ACTION_HIT_FRONT_LOWER, WLFCharacter::ACTION_HIT_FRONT_UPPER));
                }
                else {
                    sprite2->changeAction(AENSMath::randomIntBetween(WLFCharacter::ACTION_HIT_BACK_LOWER, WLFCharacter::ACTION_HIT_BACK_UPPER));		
                }
                
                if (attackJudge->effect >= 0) {
                    AESprite* spark;
                    AERO_SPRITE_DESC descSpr;
                    descSpr.obj = ae_ObjectTable.getItem(12);  // Slash Effect
                    descSpr.team = -1;  // Neutral
                    descSpr.action = attackJudge->effect;
                    descSpr.cx = collisionResult.point.x;
                    descSpr.cy = collisionResult.point.y;
                    descSpr.layerDepth = 0.0f;
                    if (sprite1->getFlip() != sprite2->getFlip()) {
                        descSpr.flip = SpriteEffects_None;
                        spark = new AESprite(descSpr);
                        spark->rotateDeg(attackJudge->angle);
                    }
                    else {
                        descSpr.flip = SpriteEffects_FlipHorizontally;
                        spark = new AESprite(descSpr);
                        spark->rotateDeg(-attackJudge->angle);
                    }
                    addSprite(spark);
                }

                dynamic_cast<WLFCharacter*>(sprite1)->setAttackLock(TRUE);
                dynamic_cast<WLFCharacter*>(sprite2)->shake(20, 1);
                setStandstill(20);

                dynamic_cast<WLFCharacter*>(sprite1)->setTarget(dynamic_cast<WLFCharacter*>(sprite2));

            }
        }
    }
}