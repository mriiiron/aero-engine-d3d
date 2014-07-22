#include "AEroEngine.h"
#include "Sprites.h"
#include "Scenes.h"
#include "AI.h"

extern AEConstantTable<AEObject>			ae_ObjectTable;

BulletAI::BulletAI(AESprite* _host) : AEAI(_host) {

}

VOID BulletAI::execute() {
	((Bullet*)host)->leaveSmoke();
}

BigSmokeEffectUnivAI::BigSmokeEffectUnivAI(AESprite* _host) : AEAI(_host) {
	smokeCount = 3;
}

VOID BigSmokeEffectUnivAI::execute() {
	if (smokeCount > 0) {
		XMFLOAT2 emitportAdjust = AENSMath::randomPointWithinCircle(10.0f);
		AERO_SPRITE_DESC descSpr;
		descSpr.obj = ae_ObjectTable.getItem(12);
		descSpr.team = host->getTeam();
		descSpr.action = 0;
		descSpr.flip = SpriteEffects_None;
		descSpr.cx = host->getCx() + emitportAdjust.x;
		descSpr.cy = host->getCy() + emitportAdjust.y;
		descSpr.layerDepth = 0.1f;
		AESprite* spr_smoke = new AESprite(descSpr);
		XMFLOAT2 randomSpeed = AENSMath::randomPointWithinCircle(0.3f);
		spr_smoke->setVx(randomSpeed.x);
		spr_smoke->setVy(randomSpeed.y);
		host->getScene()->addSprite(spr_smoke);
		smokeCount--;
	}
}

HomingAI::HomingAI(AESprite* _host, AESprite* _target) : AEAI(_host) {
	target = _target;
}

VOID HomingAI::execute() {
	FLOAT flipSpeed = ((Bullet*)host)->getSpeed();
	if (flipSpeed < 8.0 && !isLocked) {
		XMVECTOR flipVec = host->getFacingVector();
		XMFLOAT2 toTargetVecF = { target->getCx() - host->getCx(), target->getCy() - host->getCy() };
		XMVECTOR toTargetVec = XMLoadFloat2(&toTargetVecF);
		FLOAT angleBetween = XMVectorGetX(XMVector2AngleBetweenVectors(flipVec, toTargetVec));
		if (angleBetween > AENSMath::deg2rad(1.0f)) {
			FLOAT crossProduct = XMVectorGetZ(XMVector2Cross(flipVec, toTargetVec));
			host->rotateDeg(crossProduct > 0 ? 5.0f : -5.0f);
		}
		else {
			isLocked = TRUE;
		}
		((Bullet*)host)->setSpeed(flipSpeed + 0.05f);
	}
	else {
		((Bullet*)host)->setSpeed(10.0f);
	}
	host->rotateDeg(10.0f, AESprite::ANGLE_DISPLAY);
	((Bullet*)host)->leaveSmoke();
}

TurretBaseAI::TurretBaseAI(AESprite* _host) : AEAI(_host) {
	host->rotateDeg((FLOAT)AENSMath::randomIntBetween(0, 360));
}

VOID TurretBaseAI::execute() {

}

TurretAI::TurretAI(AESprite* _host) : AEAI(_host) {
	
}

VOID TurretAI::execute() {

	if (target) {

		// Chasing player
		if (host->getAction() == 0) {
			XMVECTOR flipVec = host->getFacingVector();
			XMFLOAT2 toTargetVecF = { target->getCx() - host->getCx(), target->getCy() - host->getCy() };
			XMVECTOR toTargetVec = XMLoadFloat2(&toTargetVecF);
			FLOAT angleBetween = XMVectorGetX(XMVector2AngleBetweenVectors(flipVec, toTargetVec));
			if (angleBetween > AENSMath::deg2rad(1.0f)) {
				FLOAT crossProduct = XMVectorGetZ(XMVector3Cross(flipVec, toTargetVec));
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

FlakCannonAI::FlakCannonAI(AESprite* _host) : AEAI(_host) {
	pitchAngleMax = AENSMath::deg2rad(45.0f);
	pitchAngleMin = AENSMath::deg2rad(-20.0f);
}

VOID FlakCannonAI::execute() {

	if (target) {

		// Chasing player
		if (host->getAction() == 0) {
			XMVECTOR flipVec = host->getFacingVector();
			XMFLOAT2 toTargetVecF = { target->getCx() - host->getCx(), target->getCy() - host->getCy() };
			XMVECTOR toTargetVec = XMLoadFloat2(&toTargetVecF);
			FLOAT angleBetween = XMVectorGetX(XMVector2AngleBetweenVectors(flipVec, toTargetVec));
			if (angleBetween > AENSMath::deg2rad(1.0f)) {
				FLOAT crossProduct = XMVectorGetZ(XMVector3Cross(flipVec, toTargetVec));
				host->rotateDeg(crossProduct > 0 ? 1.0f : -1.0f);
				if (host->getAngle() > pitchAngleMax) {
					host->setAngleRad(pitchAngleMax);
				}
				if (host->getAngle() < pitchAngleMin) {
					host->setAngleRad(pitchAngleMin);
				}
			}
			else {
				flakTimer = 120;
				host->changeAction(1);
			}
		}

		// Shooting at player
		else if (host->getAction() == 1) {
			if (flakTimer % 10 == 0) {
				((FlakCannon*)host)->shoot();
			}
			if (flakTimer % 5 == 0) {
				((FlakCannon*)host)->throwShell();
			}
			if (flakTimer % 17 == 0) {
				((FlakCannon*)host)->emitSmoke();
			}
			if (flakTimer > 1) flakTimer--;
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