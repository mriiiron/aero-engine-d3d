#include "AEroEngine.h"

#include "D1AI.h"
#include "D1Animation.h";
#include "D1Sprite.h"

extern AEConstantTable<AEObject>			ae_ObjectTable;


D1Sprite::D1Sprite(AERO_SPRITE_DESC desc) : AESprite(desc) {
    
}

VOID D1Sprite::update(AEHashedTable<AEPlatform>* platformTable) {
    if (ai) {
        ai->execute();
    }
    D1Animation* anim = (D1Animation*)(obj->getAnim(action));
    if (timeToLive == 0) {
        changeAction(anim->getNext());
    }
    if (timeToLive > 0) {
        timeToLive--;
    }
    time++;
    BOOLEAN isFrameChange = FALSE;
    if (time >= anim->getEndTime(frameNum)) {
        isFrameChange = TRUE;
        frameNum++;
        if (time >= anim->getEndTime(anim->getFrameCount() - 1)) {
            time = 0;
        }
        if (frameNum == anim->getFrameCount()) {
            frameNum = 0;
            if (!anim->isLoop()) {
                changeAction(anim->getNext());
            }
        }
        D1AnimOption_CreateSprite* sc = anim->getSpriteCreationOption(frameNum);
        if (sc != nullptr) {
            sc->descSprite.obj = ae_ObjectTable.getItem(sc->oid);
            sc->descSprite.team = this->team;
            sc->descSprite.cx = cx - getCurrentFrame()->getCenter().x + sc->x;
            sc->descSprite.cy = cy - getCurrentFrame()->getCenter().y + sc->y;
            D1Sprite* sprite = new D1Sprite(sc->descSprite);
            sprite->setVx(sc->vx);
            sprite->setVy(sc->vy);
            scene->addSprite(sprite);
        }
    }
    INT fac = (flip ? -1 : 1);
    FLOAT cx_old = cx, cy_old = cy;
    cx += (fac * vx);
    cy += vy;
    vx += ax;
    vy += ay;
    if (platformTable != nullptr) {
        platformCollisionCheck(cx_old, cy_old, platformTable);
    }
    if (isFrameChange) {
        isFrameChange = FALSE;
    }
    angle += (fac * vAngle);
    if (angle < -AENSMath::PI) angle += 2.0f * AENSMath::PI;
    if (angle >= AENSMath::PI) angle -= 2.0f * AENSMath::PI;
    angleDisplay += (fac * vAngleDisplay);
    if (angleDisplay < -AENSMath::PI) angleDisplay += 2.0f * AENSMath::PI;
    if (angleDisplay >= AENSMath::PI) angleDisplay -= 2.0f * AENSMath::PI;
}

VOID D1Sprite::render(INT renderOption, ...) {
    AESprite::render(renderOption);
}


D1PlayerFighter::D1PlayerFighter(AERO_SPRITE_DESC desc) : D1Sprite(desc) {
    hpMax = hpValue = 100;
    attackTimer = 0;
}

VOID D1PlayerFighter::update(AEHashedTable<AEPlatform>* platformTable) {
    
    if (!(isKeyPressed(D1Key::KEY_LEFT) && isKeyPressed(D1Key::KEY_RIGHT))) {
        FLOAT xSpeed = 0.0f;
        if (isKeyPressed(D1Key::KEY_LEFT)) {
            xSpeed = -1.0f;
        }
        else if (isKeyPressed(D1Key::KEY_RIGHT)) {
            xSpeed = 1.0f;
        }
        else {
            xSpeed = 0.0f;
        }
        setVx(xSpeed);
    }
    FLOAT ySpeed = (isKeyPressed(D1Key::KEY_UP) ? -1.0f : 0.0f) + (isKeyPressed(D1Key::KEY_DOWN) ? 1.0f : 0.0f);
    setVy(ySpeed);
    
    attackTimer--;
    if (attackTimer == 25 || attackTimer == 15 || attackTimer == 5) {
        shoot(8.0f);
        shoot(-8.0f);
    }

    if (hpValue <= 0 && action < D1PlayerFighter::ACTION_KILLED) {
        changeAction(D1PlayerFighter::ACTION_KILLED);
        AESpriteAttachment* attachedShadow = attachmentTable->getItem(0);
        if (attachedShadow) attachedShadow->getSprite()->remove();
    }
    
    D1Sprite::update(platformTable);

}

VOID D1PlayerFighter::render(INT renderOption, ...) {
    D1Sprite::render(renderOption);
}

VOID D1PlayerFighter::shoot(FLOAT xOffset) {
    AERO_SPRITE_DESC descSpr;
    descSpr.obj = ae_ObjectTable.getItem(5);
    descSpr.team = this->team;
    descSpr.action = 0;
    descSpr.flip = SpriteEffects_None;
    descSpr.cx = this->cx + xOffset;
    descSpr.cy = this->cy;
    descSpr.layerDepth = 0.001f;
    D1Sprite* vulcan = new D1Sprite(descSpr);
    vulcan->setVy(-15.0f);
    scene->addSprite(vulcan);
    descSpr.obj = ae_ObjectTable.getItem(10);
    descSpr.action = 3;
    descSpr.cx = this->cx + xOffset;
    descSpr.cy = this->cy - 7.0f;
    descSpr.layerDepth = 0.02f;
    scene->addSprite(new D1Sprite(descSpr));
}

VOID D1PlayerFighter::attack() {
    if (attackTimer <= 0) {
        attackTimer = 30;
    }
}


D1EnemyFighter::D1EnemyFighter(AERO_SPRITE_DESC desc) : D1Sprite(desc) {
    hpMax = hpValue = 50;
}

VOID D1EnemyFighter::update(AEHashedTable<AEPlatform>* platformTable) {
    if (hpValue <= 0 && action < D1PlayerFighter::ACTION_KILLED) {
        changeAction(D1EnemyFighter::ACTION_KILLED);
        AESpriteAttachment* attachedShadow = attachmentTable->getItem(0);
        if (attachedShadow) attachedShadow->getSprite()->remove();
    }
    D1Sprite::update(platformTable);
}

VOID D1EnemyFighter::render(INT renderOption, ...) {
    D1Sprite::render(renderOption);
}

VOID D1EnemyFighter::rocketAttack(FLOAT xOffset, FLOAT yOffset) {
    AERO_SPRITE_DESC descSpr;
    descSpr.obj = ae_ObjectTable.getItem(5);
    descSpr.team = this->team;
    descSpr.action = 1;
    descSpr.flip = SpriteEffects_None;
    descSpr.cx = this->cx + xOffset;
    descSpr.cy = this->cy + yOffset;
    descSpr.layerDepth = 0.01f;
    D1Sprite* rocket = new D1Sprite(descSpr);
    rocket->setVy(2.5f);
    rocket->setAI(new D1EnemyRocketAI(rocket));
    scene->addSprite(rocket);
}

VOID D1EnemyFighter::bulletAttack(FLOAT dvx, FLOAT dvy, FLOAT xOffset, FLOAT yOffset) {
    AERO_SPRITE_DESC descSpr;
    descSpr.obj = ae_ObjectTable.getItem(5);
    descSpr.team = this->team;
    descSpr.action = 4;
    descSpr.flip = SpriteEffects_None;
    descSpr.cx = this->cx + xOffset;
    descSpr.cy = this->cy + yOffset;
    descSpr.layerDepth = 0.01f;
    D1Sprite* bullet = new D1Sprite(descSpr);
    bullet->setVx(dvx);
    bullet->setVy(dvy);
    scene->addSprite(bullet);
}

VOID D1EnemyFighter::vulcanAttack() {
    AERO_SPRITE_DESC descSpr;
    descSpr.obj = ae_ObjectTable.getItem(5);
    descSpr.team = this->team;
    descSpr.action = 0;
    descSpr.flip = SpriteEffects_None;
    descSpr.cx = this->cx;
    descSpr.cy = this->cy;
    descSpr.layerDepth = 0.01f;
    D1Sprite* bullet = new D1Sprite(descSpr);
    bullet->setVy(10.0f);
    scene->addSprite(bullet);
    descSpr.obj = ae_ObjectTable.getItem(10);
    descSpr.action = 3;
    descSpr.cx = this->cx;
    descSpr.cy = this->cy + 12.0f;
    descSpr.layerDepth = 0.02f;
    scene->addSprite(new D1Sprite(descSpr));
}