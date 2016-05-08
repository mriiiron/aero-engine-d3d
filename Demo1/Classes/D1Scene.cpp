#include <dinput.h>

#include "AEroEngine.h"

#include "D1AI.h"
#include "D1Animation.h"
#include "D1Sprite.h"
#include "D1Scene.h"

extern AEConstantTable<AEResource>			ae_ResourceTable;
extern AEConstantTable<AEObject>			ae_ObjectTable;
extern AEConstantTable<AEBackground>        ae_BackgroundTable;
extern AESceneManager						ae_SceneManager;
extern AECamera                             ae_Camera;

D1Scene::D1Scene(AERO_SCENE_DESC desc) : AEScene(desc) {
    
}

VOID D1Scene::initialize() {
    dik_left = DIK_LEFT;
    dik_right = DIK_RIGHT;
    dik_up = DIK_UP;
    dik_down = DIK_DOWN;
    dik_attack = DIK_Z;
    dik_bomb = DIK_LSHIFT;
    standstill = 0;
    isStarted = FALSE;
    ae_Camera.setFocus(0.0f, 0.0f);
    reset();
}

VOID D1Scene::reset() {
    spriteTable->clear();
    level = 1;
    levelUpTimer = 1200;
    score = 0;
    addEnemyTimer = 50;
    gameOverTimer = 200;
    AERO_SPRITE_DESC descSpr;
    descSpr.obj = ae_ObjectTable.getItem(0);
    descSpr.team = 0;
    descSpr.action = 0;
    descSpr.flip = SpriteEffects_None;
    descSpr.cx = 0.0f;
    descSpr.cy = 150.0f;
    descSpr.layerDepth = 0.001f;
    D1PlayerFighter* newPlayer = new D1PlayerFighter(descSpr);
    addSprite(newPlayer);

    newPlayer->createAttachmentTable(1);
    descSpr.obj = ae_ObjectTable.getItem(10);
    descSpr.action = 5;
    descSpr.cx = 0.0f;
    descSpr.cy = 1000.0f;
    descSpr.layerDepth = 0.01f;
    D1Sprite* shadow = new D1Sprite(descSpr);
    addSprite(shadow);
    addSpriteAttachment(newPlayer, shadow, 0, AESpriteAttachment::ATTACH_MODE_STICKON);

    setPlayer(newPlayer);
}

VOID D1Scene::update() {

    AEScene::update();

    if (isPaused || !isStarted) {
        return;
    }

    if (player->getCx() < -150.0f) player->setCx(-150.0f);
    if (player->getCx() > 150.0f) player->setCx(150.0f);
    if (player->getCy() < -200.0f) player->setCy(-200.0f);
    if (player->getCy() > 200.0f) player->setCy(200.0f);
    
    if (isPlayerKilled()) {
        if (gameOverTimer > 0) {
            gameOverTimer--;
        }
    }
    else {
        addEnemyTimer--;
        if (addEnemyTimer <= 0) {
            if (getEnemyCount() < level) {
                addEnemy();
            }
            addEnemyTimer = 50;
        }
        levelUpTimer--;
        if (levelUpTimer <= 0) {
            level++;
            levelUpTimer = 1200;
        }
    }

}

VOID D1Scene::render(INT renderMode) {
    AEScene::render(renderMode);
}

VOID D1Scene::processInput() {

    if (!isPaused) {

        if (!isPlayerKilled()) {

            // Up Key (^)
            if (keyStateBuffer[dik_up] & 0x80) {
                player->keyDown(D1Key::KEY_UP);
            }
            else {
                player->keyUp(D1Key::KEY_UP);
            }

            // Down Key (v)
            if (keyStateBuffer[dik_down] & 0x80) {
                player->keyDown(D1Key::KEY_DOWN);
            }
            else {
                player->keyUp(D1Key::KEY_DOWN);
            }

            // Left Key (<)
            if (keyStateBuffer[dik_left] & 0x80) {
                if (player->getAction() == D1PlayerFighter::ACTION_MOVE_LEFT_RECOVER) {
                    INT targetFrameNum = player->getCurrentAnimation()->getFrameCount() - 1 - player->getFrameNum();
                    player->changeAction(D1PlayerFighter::ACTION_MOVE_LEFT, targetFrameNum);
                }
                else if (player->getAction() == D1PlayerFighter::ACTION_IDLE) {
                    player->changeAction(D1PlayerFighter::ACTION_MOVE_LEFT);
                }
                player->keyDown(D1Key::KEY_LEFT);
            }
            else {
                if (player->getAction() == D1PlayerFighter::ACTION_MOVE_LEFT) {
                    INT targetFrameNum = player->getCurrentAnimation()->getFrameCount() - 1 - player->getFrameNum();
                    player->changeAction(D1PlayerFighter::ACTION_MOVE_LEFT_RECOVER, targetFrameNum);
                }
                else if (player->getAction() == D1PlayerFighter::ACTION_MOVE_LEFT_HOLD) {
                    player->changeAction(D1PlayerFighter::ACTION_MOVE_LEFT_RECOVER);
                }
                player->keyUp(D1Key::KEY_LEFT);
            }

            // Right Key (>)
            if (keyStateBuffer[dik_right] & 0x80) {
                if (player->getAction() == D1PlayerFighter::ACTION_MOVE_RIGHT_RECOVER) {
                    INT targetFrameNum = player->getCurrentAnimation()->getFrameCount() - 1 - player->getFrameNum();
                    player->changeAction(D1PlayerFighter::ACTION_MOVE_RIGHT, targetFrameNum);
                }
                else if (player->getAction() == D1PlayerFighter::ACTION_IDLE) {
                    player->changeAction(D1PlayerFighter::ACTION_MOVE_RIGHT);
                }
                player->keyDown(D1Key::KEY_RIGHT);
            }
            else {
                if (player->getAction() == D1PlayerFighter::ACTION_MOVE_RIGHT) {
                    INT targetFrameNum = player->getCurrentAnimation()->getFrameCount() - 1 - player->getFrameNum();
                    player->changeAction(D1PlayerFighter::ACTION_MOVE_RIGHT_RECOVER, targetFrameNum);
                }
                else if (player->getAction() == D1PlayerFighter::ACTION_MOVE_RIGHT_HOLD) {
                    player->changeAction(D1PlayerFighter::ACTION_MOVE_RIGHT_RECOVER);
                }
                player->keyUp(D1Key::KEY_RIGHT);
            }

            if (keyStateBuffer[dik_attack] & 0x80) {
                if (!player->isKeyPressed(D1Key::KEY_ATTACK)) {
                    if (!isGameStarted()) isStarted = TRUE;
                    player->attack();
                }
                player->keyDown(D1Key::KEY_ATTACK);
            }
            else {
                player->keyUp(D1Key::KEY_ATTACK);
            }

            if (keyStateBuffer[dik_bomb] & 0x80) {
                if (!player->isKeyPressed(D1Key::KEY_BOMB)) {

                }
                player->keyDown(D1Key::KEY_BOMB);
            }
            else {
                player->keyUp(D1Key::KEY_BOMB);
            }

        }

        else if (isGameOver()) {

            if (keyStateBuffer[dik_attack] & 0x80) {
                if (!player->isKeyPressed(D1Key::KEY_ATTACK)) {
                    reset();
                }
                player->keyDown(D1Key::KEY_ATTACK);
            }
            else {
                player->keyUp(D1Key::KEY_ATTACK);
            }

        }

    }
    if (keyStateBuffer[DIK_BACKSLASH] & 0x80) {
        if (!isStepFrameKeyPressed) {
            isStepFrameKeyPressed = TRUE;
            isStepFraming = TRUE;
            togglePause();
        }
    }
    else {
        isStepFrameKeyPressed = FALSE;
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

VOID D1Scene::processCollision() {

    INT spriteCount = spriteTable->getHashCount();
    for (INT i = 0; i < spriteCount; i++) {
        AESprite* sprite1 = spriteTable->getItemByHash(i);
        if (sprite1->getObject() == nullptr) continue;
        D1Animation* anim1 = (D1Animation*)(sprite1->getCurrentAnimation());
        AERect* atkJudge = anim1->getAttackJudge(sprite1->getFrameNum());
        if (atkJudge == nullptr) continue;
        for (INT j = 0; j < spriteCount; j++) {
            AESprite* sprite2 = spriteTable->getItemByHash(j);
            if (i == j || sprite1->getTeam() == sprite2->getTeam() || sprite2->getObject() == nullptr) continue;
            D1Animation* anim2 = (D1Animation*)(sprite2->getCurrentAnimation());
            AERect* bdyJudge = anim2->getBodyJudge(sprite2->getFrameNum());
            if (bdyJudge == nullptr) continue;
            AEFrame* frame1 = anim1->getFrame(sprite1->getFrameNum());
            AEFrame* frame2 = anim2->getFrame(sprite2->getFrameNum());
            FLOAT judgeRectOffsetX1 = sprite1->getCx() - frame1->getCenter().x;
            FLOAT judgeRectOffsetX2 = sprite2->getCx() - frame2->getCenter().x;
            FLOAT judgeRectOffsetY1 = sprite1->getCy() - frame1->getCenter().y;
            FLOAT judgeRectOffsetY2 = sprite2->getCy() - frame2->getCenter().y;
            AECollisionResult collisionResult = AENSCollision::rectAndRect(AENSMath::moveRect(*atkJudge, judgeRectOffsetX1, judgeRectOffsetY1), AENSMath::moveRect(*bdyJudge, judgeRectOffsetX2, judgeRectOffsetY2));
            if (collisionResult.isCollided) {
                
                D1Sprite* dSprite1 = dynamic_cast<D1Sprite*>(sprite1);
                D1Sprite* dSprite2 = dynamic_cast<D1Sprite*>(sprite2);

                if (dSprite1->getObject()->getType() == 1) {
                    dSprite1->takeDamage(100);
                    dSprite2->takeDamage(100);
                }
                else if (dSprite1->getObject()->getType() == 2) {
                    dSprite1->remove();
                    dSprite2->takeDamage(10);

                    if (dSprite2->getObject()->getType() == 1) {
                        if (dSprite2->getHP() > 0) {
                            score += 10;
                        }
                        else {
                            score += 100;
                        }
                    }

                    AERO_SPRITE_DESC descSpr;
                    descSpr.obj = ae_ObjectTable.getItem(10);
                    descSpr.team = 10;
                    descSpr.action = 0;
                    descSpr.flip = SpriteEffects_None;
                    descSpr.cx = collisionResult.point.x;
                    descSpr.cy = collisionResult.point.y;
                    descSpr.layerDepth = 0.0f;
                    addSprite(new D1PlayerFighter(descSpr));
                }

            }
        }
    }

}

INT D1Scene::getEnemyCount() {
    INT enemyCount = 0;  // Exclude player itself
    for (INT iHash = 0; iHash < spriteTable->getHashCount(); iHash++) {
        D1Sprite* sprite = dynamic_cast<D1Sprite*>(spriteTable->getItemByHash(iHash));
        if (sprite->getObject()->getType() == 1) {
            enemyCount++;
        }
    }
    return enemyCount;
}

VOID D1Scene::addEnemy() {

    INT enemyObjID = AENSMath::randomIntBetween(1, 2);

    AERO_SPRITE_DESC descSpr;
    descSpr.obj = ae_ObjectTable.getItem(enemyObjID);
    descSpr.team = 1;
    descSpr.action = 0;
    descSpr.flip = SpriteEffects_None;
    descSpr.layerDepth = 0.001f;

    D1EnemyFighter* enemy = nullptr;

    switch (enemyObjID) {
    case 1:
        descSpr.cx = (AENSMath::randomClampf() * 2.0f - 1.0f) * 100.0f;
        descSpr.cy = -220.0f;
        enemy = new D1EnemyFighter(descSpr);
        enemy->setAI(new D1AttackerAI(enemy));
        break;
    case 2:
        descSpr.cx = (AENSMath::randomIntBetween(0, 1) * 2 - 1) * 170.0f;
        descSpr.cy = -170.0f + AENSMath::randomClampf() * 20.0f;
        enemy = new D1EnemyFighter(descSpr);
        enemy->setAI(new D1FighterAI(enemy));
        break;
    default:
        break;
    }

    addSprite(enemy);

    enemy->createAttachmentTable(1);
    descSpr.obj = ae_ObjectTable.getItem(10);
    descSpr.action = 5;
    descSpr.cx = 0.0f;
    descSpr.cy = 1000.0f;
    descSpr.layerDepth = 0.01f;
    D1Sprite* shadow = new D1Sprite(descSpr);
    addSprite(shadow);
    addSpriteAttachment(enemy, shadow, 0, AESpriteAttachment::ATTACH_MODE_STICKON);

}