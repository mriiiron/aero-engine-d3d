#include <d3d11_1.h>
#include <DirectXColors.h>
#include <stdarg.h>
#include <string>
#include <cmath>
#include "AEMath.h"
#include "AEPhysics.h"
#include "AEResource.h"
#include "AEAnimation.h"
#include "AEPlatform.h"
#include "AEBackground.h"
#include "AESprite.h"


extern AEConstantTable<AEObject>            ae_ObjectTable;
extern SpriteBatch*                         xtk_SpriteBatch;


AESpriteAttachment::AESpriteAttachment(AERO_SPRITE_ATTACHMENT_DESC desc) {
    mode = desc.mode;
    slot = desc.slot;
    depthOffset = desc.depthOffset;
    sprite = desc.sprite;
}


AESprite::AESprite(AERO_SPRITE_DESC desc) {
    scene = nullptr;
    ai = nullptr;
    attachmentTable = nullptr;
    keyState = 0;
    state = 0;
    index = frameNum = time = timeToStiff = 0;
    vx = vy = ax = ay = angle = angleDisplay = vAngle = vAngleDisplay = 0.0f;
    alpha = 1.0f;
    hpValue = hpMax = 100;
    obj = desc.obj;
    action = desc.action;
    team = desc.team;
    cx = desc.cx;
    cy = desc.cy;
    layerDepth = desc.layerDepth;
    scale = desc.scale;
    flip = desc.flip;
    deadFlag = FALSE;
    if (obj) changeAction(desc.action);
}

AEPoint AESprite::calcRotatedPoint(AEPoint point, FLOAT cx, FLOAT cy, AEFrame* f, FLOAT angle, BYTE flip) {
    FLOAT cosA = cos(angle), sinA = sin(angle);
    FLOAT x, y;
    if (flip == FACING_RIGHT) {
        x = cx + (point.x - f->getCenter().x) * cosA - (point.y - f->getCenter().y) * sinA;
        y = cy + (point.x - f->getCenter().x) * sinA + (point.y - f->getCenter().y) * cosA;
    }
    else {
        x = cx - (point.x - f->getCenter().x) * cosA + (point.y - f->getCenter().y) * sinA;
        y = cy + (point.x - f->getCenter().x) * sinA + (point.y - f->getCenter().y) * cosA;
    }
    return AEPoint(x, y);
}

AERect AESprite::calcSpriteRect(FLOAT cx, FLOAT cy, AEFrame* f, SpriteEffects flip) {
    INT centerx = f->getCenter().x, centery = f->getCenter().y;
    INT width = f->getWidth(), height = f->getHeight();
    FLOAT x1, y1, x2, y2;
    if (flip == SpriteEffects_None) {
        x1 = cx - centerx;
        y1 = cy - centery;
        x2 = x1 + width;
        y2 = y1 + height;
    }
    else {
        x2 = cx + centerx;
        y1 = cy - centery;
        x1 = x2 - width;
        y2 = y1 + height;
    }
    return AERect(x1, y1, x2, y2);
}

RECT AESprite::calcSpriteRectInRECT(FLOAT cx, FLOAT cy, AEFrame* f, SpriteEffects flip) {
    INT centerx = f->getCenter().x, centery = f->getCenter().y;
    INT width = f->getWidth(), height = f->getHeight();
    FLOAT x1, y1, x2, y2;
    if (flip == SpriteEffects_None) {
        x1 = cx - centerx;
        y1 = cy - centery;
        x2 = x1 + width;
        y2 = y1 + height;
    }
    else {
        x2 = cx + centerx;
        y1 = cy - centery;
        x1 = x2 - width;
        y2 = y1 + height;
    }
    return RECT{ (LONG)x1, (LONG)y1, (LONG)x2, (LONG)y2 };
}

AEBiasRect AESprite::calcRotatedSpriteRect(FLOAT cx, FLOAT cy, AEFrame* f, FLOAT angle, BYTE flip) {
    FLOAT cosA = cos(angle), sinA = sin(angle);
    INT centerx = f->getCenter().x, centery = f->getCenter().y;
    INT width = f->getWidth(), height = f->getHeight();
    FLOAT x1, y1, x2, y2, x3, y3, x4, y4;
    if (flip == FACING_RIGHT) {
        x1 = cx - centerx * cosA + centery * sinA;
        y1 = cy - centerx * sinA - centery * cosA;
        x2 = x1 + width * cosA;
        y2 = y1 + width * sinA;
        x3 = x2 - height * sinA;
        y3 = y2 + height * cosA;
        x4 = x1 - height * sinA;
        y4 = y1 + height * cosA;
    }
    else {
        x2 = cx + centerx * cosA - centery * sinA;
        y2 = cy - centerx * sinA - centery * cosA;
        x1 = x2 - width * cosA;
        y1 = y2 + width * sinA;
        x3 = x2 + height * sinA;
        y3 = y2 + height * cosA;
        x4 = x1 + height * sinA;
        y4 = y1 + height * cosA;
    }
    return AEBiasRect(x1, y1, x2, y2, x3, y3, x4, y4);
}

VOID AESprite::changeAction(INT targetAction, INT targetFrameNum, BOOL willResetAction) {
    if (targetAction == this->action && !willResetAction) {
        return;
    }
    if (targetAction == ACTION_NUM_DEAD) {
        deadFlag = TRUE;
        return;
    }
    AEAnimation* anim = obj->getAnim(targetAction);
    if (anim == nullptr) {
        AENSGameControl::exitGame(obj->getName() + " does not have action " + std::to_string(targetAction) + ".");
    }
    else {
        deadFlag = FALSE;
    }
    action = targetAction;
    timeToLive = anim->getTTL();
    frameNum = targetFrameNum;
    time = (targetFrameNum > 0 ? anim->getEndTime(targetFrameNum - 1) : 0);
}

XMVECTOR AESprite::getFacingVector(INT option) {
    XMFLOAT2 vec2;
    FLOAT facingAngle = 0.0f;
    if (option == ANGLE_DIRECTION) {
        facingAngle = angle;
    }
    else if (option == ANGLE_DISPLAY) {
        facingAngle = angleDisplay;
    }
    if (facingAngle == AENSMath::PI / 2) {
        vec2 = { 0.0f, 1.0f };
    }
    else if (facingAngle == -AENSMath::PI / 2) {
        vec2 = { 0.0f, -1.0f };
    }
    else if (facingAngle > -AENSMath::PI / 2 && facingAngle < AENSMath::PI / 2) {
        vec2 = { 1.0f, tanf(facingAngle) };
    }
    else {
        vec2 = { -1.0f, -tanf(facingAngle) };
    }
    if (flip == SpriteEffects_FlipHorizontally) {
        vec2.x = -vec2.x;
        vec2.y = -vec2.y;
    }
    return XMVector2Normalize(XMLoadFloat2(&vec2));
}

XMVECTOR AESprite::getVelocityVector() {
    XMFLOAT2 vec2 = { vx, vy };
    return XMVector2Normalize(XMLoadFloat2(&vec2));
}


VOID AESprite::platformCollisionCheck(FLOAT cx_old, FLOAT cy_old, AEHashedTable<AEPlatform>* platformTable) {

    XMFLOAT2 sprOldPos = { cx_old, cy_old };
    XMFLOAT2 sprNewPos = { cx_old + (flip ? -1 : 1) * vx, cy_old + vy };
    for (INT iHash = 0; iHash < platformTable->getHashCount(); iHash++) {
        AEPlatform* platform = platformTable->getItemByHash(iHash);
        for (INT i = 0; i < platform->getSegmentCount(); i++) {
            AECollisionResult collisionResult = AENSCollision::vectorAndSegment(sprOldPos, sprNewPos, platform->getNode(i), platform->getNode(i + 1));
            if (collisionResult.isCollided) {
                platformCollision(platform, i, collisionResult);
            }
        }
    }
}

VOID AESprite::platformCollision(AEPlatform* platform, INT tailNodeIndex, AECollisionResult collisionResult) {
    // By default do nothing when collide with platforms
}

VOID AESprite::update(AEHashedTable<AEPlatform>* platformTable) {
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

VOID AESprite::render(INT renderOption, ...) {

    AEFrame* f = obj->getAnim(action)->getFrame(frameNum);
    FLOAT fwidth = (FLOAT)(f->getWidth()), fcenterx = (FLOAT)(f->getCenter().x), fcentery = (FLOAT)(f->getCenter().y);
    AEResource* res = f->getResource();
    RECT texClipInTexel = res->getTexClipInTexel(f->getImgOffset(), f->getImgCells());
    FLOAT flipAdjust = flip * (fwidth - 2.0f * fcenterx);
    FLOAT flipAdjustX = flipAdjust * cosf(angleDisplay), flipAdjustY = flipAdjust * sinf(angleDisplay);

    va_list va;
    va_start(va, renderOption);

    switch (renderOption) {
    case RENDER_OPTION_NORMAL:
        xtk_SpriteBatch->Draw(
            res->getTexture(), // Texture
            XMFLOAT2(INT(cx - flipAdjustX), INT(cy - flipAdjustY)), // Drawing Destination Position (Origin Point)
            &texClipInTexel, // Texture Clip Rectangle
            XMVectorSet(1.0f, 1.0f, 1.0f, alpha), // Tilting Color
            angleDisplay, // Rotation
            XMFLOAT2(fcenterx, fcentery), // Rotation Origin / Drawing Center
            scale, // Scale
            flip, // Sprite Effects
            layerDepth // Z Value
            );
        break;
    case RENDER_OPTION_SCALE:
    {
        FLOAT optionScale = (FLOAT)(va_arg(va, DOUBLE));
        xtk_SpriteBatch->Draw(
            res->getTexture(), // Texture
            XMFLOAT2(INT(cx - flipAdjustX), INT(cy - flipAdjustY)), // Drawing Destination Position (Origin Point)
            &texClipInTexel, // Texture Clip Rectangle
            XMVectorSet(1.0f, 1.0f, 1.0f, alpha), // Tilting Color
            angleDisplay, // Rotation
            XMFLOAT2(fcenterx, fcentery), // Rotation Origin / Drawing Center
            optionScale, // Scale
            flip, // Sprite Effects
            layerDepth // Z Value
            );
    }
        break;
    case RENDER_OPTION_WIPE:
    {
        INT wipeDirection = va_arg(va, INT);
        FLOAT wipePercent = (FLOAT)(va_arg(va, DOUBLE));
        RECT destRect = calcSpriteRectInRECT(cx, cy, f, flip);
        switch (wipeDirection) {
        case RENDER_WIPE_LEFT:
            destRect.right = (INT)(destRect.left + wipePercent * (destRect.right - destRect.left));
            texClipInTexel.right = (INT)(texClipInTexel.left + wipePercent * (texClipInTexel.right - texClipInTexel.left));
            break;
        case RENDER_WIPE_RIGHT:
            destRect.left = (INT)(destRect.right - wipePercent * (destRect.right - destRect.left));
            texClipInTexel.left = (INT)(texClipInTexel.right - wipePercent * (texClipInTexel.right - texClipInTexel.left));
            break;
        case RENDER_WIPE_TOP:
            break;
        case RENDER_WIPE_BOTTOM:
            break;
        default:
            va_end(va);
            AENSGameControl::exitGame("Error: Invalid render option.");
        }
        xtk_SpriteBatch->Draw(
            res->getTexture(), // Texture
            destRect, // Drawing Destination Rectangle
            &texClipInTexel, // Texture Clip Rectangle
            XMVectorSet(1.0f, 1.0f, 1.0f, alpha), // Tilting Color
            angleDisplay, // Rotation
            XMFLOAT2(0.0f, 0.0f), // Rotation Origin / Drawing Center
            flip, // Sprite Effects
            layerDepth // Z Value
            );
    }
        break;
    case RENDER_OPTION_OFFSET:
    {
        INT xOffset = va_arg(va, INT);
        INT yOffset = va_arg(va, INT);
        xtk_SpriteBatch->Draw(
            res->getTexture(), // Texture
            XMFLOAT2(INT(cx - flipAdjustX + xOffset), INT(cy - flipAdjustY + yOffset)), // Drawing Destination Position (Origin Point)
            &texClipInTexel, // Texture Clip Rectangle
            XMVectorSet(1.0f, 1.0f, 1.0f, alpha), // Tilting Color
            angleDisplay, // Rotation
            XMFLOAT2(fcenterx, fcentery), // Rotation Origin / Drawing Center
            scale, // Scale
            flip, // Sprite Effects
            layerDepth // Z Value
            );
    }
        break;
    case RENDER_OPTION_TILT:
    {
        FLOAT r = (FLOAT)(va_arg(va, DOUBLE));
        FLOAT g = (FLOAT)(va_arg(va, DOUBLE));
        FLOAT b = (FLOAT)(va_arg(va, DOUBLE));
        xtk_SpriteBatch->Draw(
            res->getTexture(), // Texture
            XMFLOAT2(INT(cx - flipAdjustX), INT(cy - flipAdjustY)), // Drawing Destination Position (Origin Point)
            &texClipInTexel, // Texture Clip Rectangle
            XMVectorSet(r, g, b, alpha), // Tilting Color
            angleDisplay, // Rotation
            XMFLOAT2(fcenterx, fcentery), // Rotation Origin / Drawing Center
            scale, // Scale
            flip, // Sprite Effects
            layerDepth // Z Value
            );
    }
        break;
    default:
        va_end(va);
        AENSGameControl::exitGame("Error: Invalid render option.");
    }

    va_end(va);
    
    // Old drawing method not using SpriteBatch
    /*
    AERect texClip = res->getTexClip(f->getImgOffset(), f->getImgCells(), flip);
    if (angle == 0.0f) {
        AERect paintArea = calcSpriteRect(cx, cy, f, flip);
        res->addToRenderBuffer(paintArea, texClip, alpha, zValue);
    }
    else {
        AEBiasRect paintArea = calcRotatedSpriteRect(cx, cy, f, angle, flip);
        res->addToRenderBuffer(paintArea, texClip, alpha, zValue);
    }
    */
}

FLOAT AESprite::getAngle(INT option) {
    if (option == ANGLE_DIRECTION) {
        return angle;
    }
    else {
        return angleDisplay;
    }
}

FLOAT AESprite::getVAngle(INT option) {
    if (option == ANGLE_DIRECTION) {
        return vAngle;
    }
    else {
        return vAngleDisplay;
    }
}

VOID AESprite::setAngleRad(FLOAT rad, INT option) {
    if (option != ANGLE_DISPLAY) {
        angle = rad;
    }
    if (option != ANGLE_DIRECTION) {
        angleDisplay = rad;
    }
}

VOID AESprite::setAngleDeg(FLOAT degree, INT option) {
    setAngleRad(AENSMath::deg2rad(degree), option);
}

VOID AESprite::setVAngleRad(FLOAT rad, INT option){
    if (option != ANGLE_DISPLAY) {
        vAngle = rad;
    }
    if (option != ANGLE_DIRECTION) {
        vAngleDisplay = rad;
    }
}

VOID AESprite::setVAngleDeg(FLOAT degree, INT option){
    setVAngleRad(AENSMath::deg2rad(degree), option);
}

VOID AESprite::move(FLOAT dx, FLOAT dy, INT option) {
    FLOAT cosA, sinA;
    switch (option) {
    case MOVE_IGNORE_DIRECTION:
        cx += dx;
        cy += dy;
        break;
    case MOVE_ALONG_DIRECTION:
        cosA = cos(angle), sinA = sin(angle);
        cx += dx * cosA - dy * sinA;
        cy += dx * sinA + dy * cosA;
        break;
    default:
        break;
    }
}

VOID AESprite::rotateRad(FLOAT rad, INT option) {
    if (option != ANGLE_DISPLAY) {
        angle += rad;
    }
    if (option != ANGLE_DIRECTION) {
        angleDisplay += rad;
    }
}

VOID AESprite::rotateDeg(FLOAT degree, INT option) {
    rotateRad(AENSMath::deg2rad(degree), option);
}

VOID AESprite::createAttachmentTable(INT size) {
    attachmentTable = new AEHashedTable<AESpriteAttachment>(size);
    attachmentTableSize = size;
}

VOID AESprite::updateAttachments() {
    for (INT iHash = 0; iHash < attachmentTable->getHashCount(); iHash++) {
        AESpriteAttachment* attachment = attachmentTable->getItemByHash(iHash);
        AESprite* attachedSprite = attachment->getSprite();
        if (attachedSprite->isDead()) {
            attachmentTable->removeItemByHash(iHash, MEMORY_NOT_RELEASE);
        }
        else {
            INT fac;
            AEFrame* currentFrame = getCurrentFrame();
            AEPointI attachPoint = currentFrame->getAttachPoint(attachment->getSlot());
            switch (attachment->getMode()) {
            case AESpriteAttachment::ATTACH_MODE_FOLLOW:
                fac = (flip ? -1 : 1);
                attachedSprite->move(fac * vx, vy);
                // TODO: Process Attachment Rotation
                //attachedSprite->rotateRad(fac * vAngle, ANGLE_DIRECTION);
                //if (attachedSprite->getAngle(ANGLE_DIRECTION) < -AENSMath::PI) attachedSprite->rotateRad(2.0f * AENSMath::PI, ANGLE_DIRECTION);
                //if (attachedSprite->getAngle(ANGLE_DIRECTION) >= AENSMath::PI) attachedSprite->rotateRad(-2.0f * AENSMath::PI, ANGLE_DIRECTION);
                //attachedSprite->rotateRad(fac * vAngleDisplay, ANGLE_DISPLAY);
                //if (attachedSprite->getAngle(ANGLE_DISPLAY) < -AENSMath::PI) attachedSprite->rotateRad(2.0f * AENSMath::PI, ANGLE_DISPLAY);
                //if (attachedSprite->getAngle(ANGLE_DISPLAY) >= AENSMath::PI) attachedSprite->rotateRad(-2.0f * AENSMath::PI, ANGLE_DISPLAY);
                break;
            case AESpriteAttachment::ATTACH_MODE_STICKON:
                attachedSprite->setCx(cx + attachPoint.x - currentFrame->getCenter().x);
                attachedSprite->setCy(cy + attachPoint.y - currentFrame->getCenter().y);
                break;
            default:
                AENSGameControl::exitGame("Illegal Sprite Attachment Mode.");
                break;
            }
        }
    }
}