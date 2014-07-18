#include "AEroEngine.h"
#include "Sprites.h"
#include "Scenes.h"
#include "AI.h"

extern AEConstantTable<AEObject>			ae_ObjectTable;

BulletAI::BulletAI(AESprite* _host) : AEAI(_host) {

}

RocketAI::RocketAI(AESprite* _host) : BulletAI(_host) {
	smokeCooldown = 0;
	((Bullet*)host)->setFacingSpeed(5.0f);
}

VOID BulletAI::leaveSmoke() {
	AERO_SPRITE_DESC descSpr;
	descSpr.obj = ae_ObjectTable.getItem(3);
	descSpr.team = 0;
	descSpr.action = 0;
	descSpr.facing = SpriteEffects_None;
	descSpr.cx = host->getCx();
	descSpr.cy = host->getCy();
	AESprite* spr_smoke = new AESprite(descSpr);
	XMFLOAT2 randomSpeed = AENSMath::randomPointWithinCircle(2.0f);
	spr_smoke->setVx(randomSpeed.x);
	spr_smoke->setVy(randomSpeed.y);
	host->getScene()->addSprite(spr_smoke);
}

VOID RocketAI::execute() {
	if (smokeCooldown <= 0) {
		((Bullet*)host)->setFacingSpeed(10.0f);
		leaveSmoke();
		smokeCooldown = 0;
	}
	else {
		smokeCooldown--;
	}
}

HomingAI::HomingAI(AESprite* _host, AESprite* _target) : BulletAI(_host) {
	target = _target;
}

VOID HomingAI::execute() {
	FLOAT facingSpeed = ((Bullet*)host)->getFacingSpeed();
	if (facingSpeed < 8.0 && !isLocked) {
		XMVECTOR facingVec = host->getFacingVector();
		XMFLOAT2 toTargetVecF = { target->getCx() - host->getCx(), target->getCy() - host->getCy() };
		XMVECTOR toTargetVec = XMLoadFloat2(&toTargetVecF);
		FLOAT angleBetween = XMVectorGetX(XMVector2AngleBetweenVectors(facingVec, toTargetVec));
		if (angleBetween > AENSMath::deg2rad(1.0f)) {
			FLOAT crossProduct = XMVectorGetZ(XMVector2Cross(facingVec, toTargetVec));
			host->rotateDeg(crossProduct > 0 ? 5.0f : -5.0f);
		}
		else {
			isLocked = TRUE;
		}
		((Bullet*)host)->setFacingSpeed(facingSpeed + 0.05f);
	}
	else {
		((Bullet*)host)->setFacingSpeed(10.0f);
	}
	host->rotateDeg(10.0f, AESprite::ROTATE_DISPLAY_ONLY);
	leaveSmoke();
}

TurretBaseAI::TurretBaseAI(AESprite* _host) : AEAI(_host) {
	host->rotateDeg((FLOAT)AENSMath::randomIntBetween(0, 360));
	host->createAttachmentTable(1);
	AERO_SPRITE_DESC descSpr;
	descSpr.obj = ae_ObjectTable.getItem(5);
	descSpr.team = 1;
	descSpr.action = 0;
	descSpr.facing = SpriteEffects_None;
	descSpr.cx = host->getCx();
	descSpr.cy = host->getCy();
	Turret* spr_turret = new Turret(descSpr);
	spr_turret->setAI(new TurretAI(spr_turret));
	host->addAttachment(0, spr_turret);
}

VOID TurretBaseAI::execute() {

}

TurretAI::TurretAI(AESprite* _host) : AEAI(_host) {
	
}

VOID TurretAI::execute() {

	if (target) {

		// Chasing player
		if (host->getAction() == 0) {
			XMVECTOR facingVec = host->getFacingVector();
			XMFLOAT2 toTargetVecF = { target->getCx() - host->getCx(), target->getCy() - host->getCy() };
			XMVECTOR toTargetVec = XMLoadFloat2(&toTargetVecF);
			FLOAT angleBetween = XMVectorGetX(XMVector2AngleBetweenVectors(facingVec, toTargetVec));
			if (angleBetween > AENSMath::deg2rad(1.0f)) {
				FLOAT crossProduct = XMVectorGetZ(XMVector3Cross(facingVec, toTargetVec));
				host->rotateDeg(crossProduct > 0 ? 1.0f : -1.0f);
			}
			else {
				homingFired = 0;
				host->changeAction(1);
			}
		}

		// Shooting at player
		else if (host->getAction() == 1) {
			if (!homingFired) {
				((Turret*)host)->shootHomingBullets();
				homingFired = 1;
			}
			if (rocketCooldown <= 0) {
				((Turret*)host)->shootRocket();
				rocketCooldown = 20;
			}
			rocketCooldown--;
		}

		// Cooling down
		else if (host->getAction() == 2) {
			// Do nothing
		}

	}

	else {
		VerticalScrollerScene* scene = (VerticalScrollerScene*)(host->getScene());
		AEHashedTable<AESprite>* spriteTable = scene->getSpriteTable();
		for (INT iHash = 0; iHash < spriteTable->getHashCount(); iHash++) {
			AESprite* sprite = spriteTable->getItemByHash(iHash);
			if (!sprite->isDead() && sprite->getTeam() != host->getTeam()) {
				target = sprite;
				break;
			}
		}
	}
}