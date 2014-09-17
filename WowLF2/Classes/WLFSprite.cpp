#include "AEroEngine.h"
#include "WLFAI.h"
#include "WLFAnimation.h"
#include "WLFSprite.h"
#include "WLFScene.h"

extern AEConstantTable<AEObject>			ae_ObjectTable;


WLFCharacter::WLFCharacter(AERO_SPRITE_DESC desc) : AESprite(desc) {
	onPlatform = nullptr;
	onPlatformTailIndex = 0;
	isRightKeyPressed = isLeftKeyPressed = FALSE;
	attackLock = FALSE;
}

VOID WLFCharacter::platformCollision(AEPlatform* platform, INT tailNodeIndex, AECollisionResult collisionResult) {
	AESprite::platformCollision(platform, tailNodeIndex, collisionResult);
	onPlatform = platform;
	onPlatformTailIndex = tailNodeIndex;
	state = STATE_ON_GROUND;
	changeAction(9);
	ay = 0.0f;
	vy = 0.0f;
	adsorbToPlatform();
}

//  Overrides AESprite::update() completely
VOID WLFCharacter::update(AEHashedTable<AEPlatform>* platformTable) {
	if (state == STATE_IN_AIR) {
		changeAction(8);
		ay = ((WLFShrineCaveScene*)scene)->GRAVITY;
	}
	if (state == STATE_ON_GROUND) {
		if (action == 0) {
			if (isRightKeyPressed && !isLeftKeyPressed) {
				changeAction(2);
				flip = SpriteEffects::SpriteEffects_None;
				setVx(2.0f);
			}
			else if (!isRightKeyPressed && isLeftKeyPressed) {
				changeAction(2);
				flip = SpriteEffects::SpriteEffects_FlipHorizontally;
				setVx(2.0f);
			}
		}
		else if (action == 2) {
			if (isRightKeyPressed == isLeftKeyPressed) {
				changeAction(0);
				setVx(0.0f);
			}
		}
	}
	if (ai) {
		ai->execute();
	}
	WLFAnimation* anim = (WLFAnimation*)(obj->getAnim(action));
	if (timeToLive == 0) {
		changeAction(anim->getNext());
	}
	if (timeToLive > 0) {
		timeToLive--;
	}
	time++;
	BOOLEAN isFrameChange = FALSE;
	INT fac = (flip ? -1 : 1);
	if (time >= anim->getEndTime(frameNum)) {
		isFrameChange = TRUE;
		if (attackLock) attackLock = FALSE;
		frameNum++;
		if (time >= anim->getEndTime(anim->getFrameCount() - 1)) {

			time = 0;
		}
		vx += anim->getXShift(frameNum);
		WLFSpriteCreatePoint* spriteCreate = anim->getSpriteCreate(frameNum);
		if (spriteCreate != nullptr) {
			spriteCreate->descSprite.obj = ae_ObjectTable.getItem(spriteCreate->oid);
			spriteCreate->descSprite.cx = this->cx - fac * (getCurrentFrame()->getCenterx() - spriteCreate->x);
			spriteCreate->descSprite.cy = this->cy - getCurrentFrame()->getCentery() + spriteCreate->y;
			switch (spriteCreate->flip) {
			case WLFSpriteCreatePoint::FLIP_ALWAYS_RIGHT:
				spriteCreate->descSprite.flip = SpriteEffects_None;
				break;
			case WLFSpriteCreatePoint::FLIP_ALWAYS_LEFT:
				spriteCreate->descSprite.flip = SpriteEffects_FlipHorizontally;
				break;
			case WLFSpriteCreatePoint::FLIP_OPPOSITE_TO_HOST:
				spriteCreate->descSprite.flip = (this->flip == SpriteEffects_None ? SpriteEffects_FlipHorizontally : SpriteEffects_None);
				break;
			default:
				spriteCreate->descSprite.flip = this->flip;
				break;
			}
			scene->addSprite(new AESprite(spriteCreate->descSprite));
		}
		if (frameNum == anim->getFrameCount()) {
			frameNum = 0;
			if (!anim->isLoop()) {
				changeAction(anim->getNext());
			}
		}
	}
	if (state == STATE_IN_AIR) {
		FLOAT cx_old = cx, cy_old = cy;
		cx += (fac * vx);
		cy += vy;
		if (platformTable != nullptr) {
			platformCollisionCheck(cx_old, cy_old, platformTable);
		}
	}
	else if (state == STATE_ON_GROUND) {
		cx += (fac * vx) * cosf(onPlatform->getAngleRad(onPlatformTailIndex));
		cy += (fac * vx) * sinf(onPlatform->getAngleRad(onPlatformTailIndex));
		if (cx > onPlatform->getNode(onPlatformTailIndex + 1).x) {
			onPlatformTailIndex++;
			if (onPlatformTailIndex >= onPlatform->getNodeCount() - 1) {
				// TODO: Fall out
			}
			adsorbToPlatform();
		}
		else if (cx < onPlatform->getNode(onPlatformTailIndex).x) {
			onPlatformTailIndex--;
			if (onPlatformTailIndex < 0) {
				// TODO: Fall out
			}
			adsorbToPlatform();
		}
		if (vy != 0.0f) {
			// TODO: Jump out of a platform
		}
	}
	vx += ax;
	vy += ay;
	if (isFrameChange) {
		vx -= anim->getXShift(frameNum);
		isFrameChange = FALSE;
	}
	angle += (fac * vAngle);
	if (angle < -AENSMath::PI) angle += 2.0f * AENSMath::PI;
	if (angle >= AENSMath::PI) angle -= 2.0f * AENSMath::PI;
	angleDisplay += (fac * vAngleDisplay);
	if (angleDisplay < -AENSMath::PI) angleDisplay += 2.0f * AENSMath::PI;
	if (angleDisplay >= AENSMath::PI) angleDisplay -= 2.0f * AENSMath::PI;
	if (hasAttachments()) {
		updateAttachments(fac* vx, vy);
	}
}

VOID WLFCharacter::toBattleStance() {
	if (this->getAction() == 0) {
		this->changeAction(4);
	}
}

VOID WLFCharacter::toStand() {
	if (this->getAction() == 1) {
		this->changeAction(5);
	}
}

VOID WLFCharacter::attack_1() {
	if (this->getAction() == 1) {
		this->changeAction(13);
	}
}

VOID WLFCharacter::attack_2() {
	if (this->getAction() == 1) {
		this->changeAction(14);
	}
}

VOID WLFCharacter::adsorbToPlatform() {
	XMFLOAT2 tail = onPlatform->getNode(onPlatformTailIndex);
	XMFLOAT2 head = onPlatform->getNode(onPlatformTailIndex + 1);
	if (cx == head.x) {
		cy = head.y;
		return;
	}
	else {
		FLOAT ratio = (tail.x - cx) / (head.x - cx);
		cy = (ratio * head.y - tail.y) / (ratio - 1);
	}
}