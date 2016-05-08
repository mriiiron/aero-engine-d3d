#include "AEroEngine.h"
#include "D1Sprite.h"
#include "D1Scene.h"
#include "D1AI.h"

extern AEConstantTable<AEObject>			ae_ObjectTable;


D1AttackerAI::D1AttackerAI(AESprite* _host) : AEAI(_host) {
    actionTimer = 100;
    bulletTimer = 50;
    rocketLockTimer = 1;
    secondRocketTimer = 0;
    action = 0;
    _host->setVy(0.8f);
}

VOID D1AttackerAI::execute() {

    D1EnemyFighter* me = dynamic_cast<D1EnemyFighter*>(host);
    D1PlayerFighter* player = dynamic_cast<D1Scene*>(host->getScene())->getPlayer();

    rocketLockTimer--;
    if (rocketLockTimer <= 0 && fabs(me->getCx() - player->getCx()) < 15.0f) {
        me->rocketAttack(3.0f, 0.0f);
        rocketLockTimer = 80;
        secondRocketTimer = 10;
    }

    if (secondRocketTimer > 0) {
        secondRocketTimer--;
        if (secondRocketTimer <= 0) {
            me->rocketAttack(-3.0f, 0.0f);
        }
    }

    if (player->getAction() < D1PlayerFighter::ACTION_KILLED) {
        bulletTimer--;
        if (bulletTimer <= 0) {
            FLOAT angleRad = atan2f(player->getCy() - me->getCy(), player->getCx() - me->getCx());
            me->bulletAttack(4.0f * cosf(angleRad), 4.0f * sinf(angleRad));
            bulletTimer = 50;
        }
    }

    actionTimer--;
    if (actionTimer <= 0) {
        switch (action)
        {
        case 0:
            me->changeAction(D1EnemyFighter::ACTION_SPIN);
            me->setAx(me->getCx() > player->getCx() ? -0.015f : 0.015f);
            actionTimer = 75;
            break;
        case 1:
            me->setAx(-me->getAx());
            actionTimer = 75;
            break;
        case 2:
            me->changeAction(D1EnemyFighter::ACTION_SPIN_RECOVER, me->getFrameNum());
            me->setAx(0.0f);
            me->setVx(0.0f);
            break;
        default:
            break;
        }
        action++;
    }

    if (me->getCy() > 250.0f) {
        me->remove();
    }

}


D1EnemyRocketAI::D1EnemyRocketAI(AESprite * _host) : AEAI(_host) {

}

VOID D1EnemyRocketAI::execute() {
    D1Sprite* me = dynamic_cast<D1Sprite*>(host);
    D1PlayerFighter* player = dynamic_cast<D1Scene*>(host->getScene())->getPlayer();
    if (me->getAction() != 2 && AENSMath::euclideanDistance(player->getCenter(), me->getCenter()) < 50.0f) {
        me->changeAction(2);
    }
    if (me->getAction() == 2) {
        me->setAy(-0.12f);
    }
}


D1FighterAI::D1FighterAI(AESprite * _host) : AEAI(_host) {
    vulcanLockTimer = vulcanTimer = 0;
    actionTimer = 100;
    action = 0;
}

VOID D1FighterAI::execute() {

    D1EnemyFighter* me = dynamic_cast<D1EnemyFighter*>(host);
    D1PlayerFighter* player = dynamic_cast<D1Scene*>(host->getScene())->getPlayer();

    vulcanLockTimer--;
    if (vulcanLockTimer <= 0 && vulcanTimer <= 0 && fabs(me->getCx() - player->getCx()) < 10.0f) {
        vulcanTimer = 60;
        vulcanLockTimer = 150;
    }
    vulcanTimer--;
    if (vulcanTimer > 0 && vulcanTimer % 10 == 0) {
        me->vulcanAttack();
    }

    actionTimer--;
    if (actionTimer <= 0) {
        switch (action)
        {
        case 0:
        case 2:
            me->changeAction(D1EnemyFighter::ACTION_SPIN);
            me->setAx(2.0f * (me->getCx() - player->getCx()) / 2500.0f);
            me->setVx(-50.0f * me->getAx());
            me->setAy(2.0f * AENSMath::randomIntBetween(-1, 1) * 20.0f / 2500.0f);
            me->setVy(-50.0f * me->getAy());
            actionTimer = 50;
            break;
        case 1:
        case 3:
            me->setAx(0.0f);
            me->setAy(0.0f);
            me->setVx(0.0f);
            me->setVy(0.0f);
            actionTimer = 150;
            break;
        case 4:
            me->rocketAttack(0.0f, -8.0f);
            me->setVy(2.5f);
        default:
            break;
        }
        action++;
    }

    if (me->getCy() > 250.0f) {
        me->remove();
    }

}
