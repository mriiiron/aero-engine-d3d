#pragma once


class D1AttackerAI : public AEAI {

public:

    D1AttackerAI(AESprite* _host);
    VOID execute();

private:

    INT actionTimer, action;
    INT bulletTimer;
    INT rocketLockTimer, secondRocketTimer;

};


class D1FighterAI : public AEAI {

public:

    D1FighterAI(AESprite* _host);
    VOID execute();

private:

    INT actionTimer, action;
    INT vulcanTimer, vulcanLockTimer;

};


class D1EnemyRocketAI : public AEAI {

public:

    D1EnemyRocketAI(AESprite* _host);
    VOID execute();

private:



};