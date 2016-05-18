#include "AEroEngine.h"
#include "D2AI.h"
#include "D2Sprite.h"
#include "D2Scene.h"

extern AEConstantTable<AEObject>			ae_ObjectTable;


D2Sprite::D2Sprite(AERO_SPRITE_DESC desc) : AESprite(desc) {
    onPlatform = nullptr;
    onPlatformTailIndex = 0;
    moving = 0;
}

VOID D2Sprite::platformCollision(AEPlatform * platform, INT tailNodeIndex, AECollisionResult collisionResult) {
    AESprite::platformCollision(platform, tailNodeIndex, collisionResult);
    onPlatform = platform;
    onPlatformTailIndex = tailNodeIndex;
    FLOAT platformAngle = onPlatform->getAngleRad(onPlatformTailIndex);
    state = STATE_ON_GROUND;
    ay = vy = 0.0f;
    if (moving == 0) {
        ax = vx = 0.0f;
        changeAction(D2Sprite::ACTION_IDLE);
    }
    else {
        changeAction(D2Sprite::ACTION_RUN);
    }
    absorbToPlatform();
}

VOID D2Sprite::absorbToPlatform() {
    XMFLOAT2 tail = onPlatform->getNode(onPlatformTailIndex);
    XMFLOAT2 head = onPlatform->getNode(onPlatformTailIndex + 1);
    FLOAT x1, x2;
    if (tail.x < head.x) {
        x1 = tail.x;  x2 = head.x;
    }
    else {
        x1 = head.x;  x2 = tail.x;
    }
    if (cx < x1) cx = x1;
    if (cx > x2) cx = x2;
    if (x1 == x2) {
        return;
    }
    if (cx == head.x) {
        cy = head.y;
        return;
    }
    else {
        FLOAT ratio = (tail.x - cx) / (head.x - cx);
        cy = (ratio * head.y - tail.y) / (ratio - 1);
    }
}

VOID D2Sprite::update(AEHashedTable<AEPlatform>* platformTable) {
    if (state == STATE_IN_AIR) {
        onPlatform = nullptr;
        onPlatformTailIndex = 0;
        ay = dynamic_cast<D2Scene*>(scene)->GRAVITY;
    }
    
    if (state == STATE_IN_AIR) {
        if (isKeyPressed(D2Keys::KEY_RIGHT) && !isKeyPressed(D2Keys::KEY_LEFT)) {
            if (flip == SpriteEffects::SpriteEffects_FlipHorizontally) {
                turnOverHorizontally();
            }
            moving = 1;
        }
        else if (isKeyPressed(D2Keys::KEY_LEFT) && !isKeyPressed(D2Keys::KEY_RIGHT)) {
            if (flip == SpriteEffects::SpriteEffects_None) {
                turnOverHorizontally();
            }
            moving = -1;
        }
        else {
            moving = 0;
        }
    }
    else if (state == STATE_ON_GROUND) {
        if (isKeyPressed(D2Keys::KEY_JUMP)) {
            vy = -4.0f;
            state = STATE_IN_AIR;
        }
        if (action == D2Sprite::ACTION_IDLE) {
            if (isKeyPressed(D2Keys::KEY_RIGHT) && !isKeyPressed(D2Keys::KEY_LEFT)) {
                if (flip == SpriteEffects::SpriteEffects_FlipHorizontally) {
                    turnOverHorizontally();
                }
                changeAction(D2Sprite::ACTION_RUN);
                moving = 1;
            }
            else if (isKeyPressed(D2Keys::KEY_LEFT) && !isKeyPressed(D2Keys::KEY_RIGHT)) {
                if (flip == SpriteEffects::SpriteEffects_None) {
                    turnOverHorizontally();
                }
                changeAction(D2Sprite::ACTION_RUN);
                moving = -1;
            }
        }
        else if (action == D2Sprite::ACTION_RUN) {
            if (isKeyPressed(D2Keys::KEY_LEFT) == isKeyPressed(D2Keys::KEY_RIGHT) || (flip == SpriteEffects::SpriteEffects_None && isKeyPressed(D2Keys::KEY_LEFT) && !isKeyPressed(D2Keys::KEY_RIGHT)) || (flip == SpriteEffects::SpriteEffects_FlipHorizontally && isKeyPressed(D2Keys::KEY_RIGHT) && !isKeyPressed(D2Keys::KEY_LEFT))) {
                changeAction(D2Sprite::ACTION_IDLE);
                moving = 0;
            }
        }
    }
    else {
        AENSGameControl::exitGame("Invalid sprite state.");
    }

    if (ai) {
        ai->execute();
    }

    AEAnimation* anim = obj->getAnim(action);
    if (timeToLive == 0) {
        changeAction(anim->getNext());
    }
    if (timeToLive > 0) {
        timeToLive--;
    }
    time++;
    BOOLEAN isFrameChange = FALSE;
    if (time >= anim->getEndTime(frameNum)) {
        isFrameChange = FALSE;
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
    }
    INT fac = (flip ? -1 : 1);
    if (state == STATE_IN_AIR) {
        if (action != D2Sprite::ACTION_JUMP) {
            changeAction(D2Sprite::ACTION_JUMP);
        }
        FLOAT cx_old = cx, cy_old = cy;
        cx += (fac * vx);
        cy += vy;
        if (platformTable != nullptr) {
            platformCollisionCheck(cx_old, cy_old, platformTable);
        }

        FLOAT fraction = dynamic_cast<D2Scene*>(scene)->FRACTION;
        if (moving == 0) {
            if (fabs(vx) <= fraction) {
                vx = ax = 0.0f;
            }
            else {
                ax = fraction * ((vx < 0.0f) - (vx > 0.0f));
            }
        }
        else {
            ax = fabs(moving) * 0.02f;
        }
    }
    else if (state == STATE_ON_GROUND) {
        cx += (fac * vx) * cosf(onPlatform->getAngleRad(onPlatformTailIndex));
        cy += (fac * vx) * sinf(onPlatform->getAngleRad(onPlatformTailIndex));
        if (cx > onPlatform->getNode(onPlatformTailIndex + 1).x) {
            onPlatformTailIndex++;
            if (onPlatformTailIndex >= onPlatform->getNodeCount() - 1 || onPlatform->getAngleRad(onPlatformTailIndex) > AENSMath::deg2rad(60.0f)) {
                state = STATE_IN_AIR;
            }
            else if (onPlatform->getAngleRad(onPlatformTailIndex) < AENSMath::deg2rad(-60.0f)) {
                onPlatformTailIndex--;
                cx = onPlatform->getNode(onPlatformTailIndex + 1).x;
                absorbToPlatform();
            }
            else {
                absorbToPlatform();
            }
            
        }
        else if (cx < onPlatform->getNode(onPlatformTailIndex).x) {
            onPlatformTailIndex--;
            if (onPlatformTailIndex < 0 || onPlatform->getAngleRad(onPlatformTailIndex) < AENSMath::deg2rad(-60.0f)) {
                state = STATE_IN_AIR;
            }
            else if (onPlatform->getAngleRad(onPlatformTailIndex) > AENSMath::deg2rad(60.0f)) {
                onPlatformTailIndex++;
                cx = onPlatform->getNode(onPlatformTailIndex).x;
                absorbToPlatform();
            }
            else {
                absorbToPlatform();
            }
        }
        
        FLOAT fraction = dynamic_cast<D2Scene*>(scene)->FRACTION;
        if (moving == 0) {
            if (fabs(vx) <= fraction) {
                vx = ax = 0.0f;
            }
            else {
                ax = fraction * ((vx < 0.0f) - (vx > 0.0f));
            }
        }
        else {
            ax = fabs(moving) * 0.05f;
        }

        if (vy < 0.0f) {
            state = STATE_IN_AIR;
        }

    }
    else {
        AENSGameControl::exitGame("Invalid sprite state.");
    }

    if (cx < -56.0f) cx = -56.0f;
    if (cx > 504.0f) cx = 504.0f;

    vx += ax;
    vy += ay;

    if (vx > 2.0f) vx = 2.0f;

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

VOID D2Sprite::render(INT renderOption, ...) {
    AESprite::render(renderOption);
}
