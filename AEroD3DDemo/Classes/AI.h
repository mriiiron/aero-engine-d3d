#pragma once


class BulletAI : public AEAI {
public:
	BulletAI(AESprite* _host);
protected:
	VOID leaveSmoke();
};


class RocketAI : public BulletAI {
public:
	RocketAI(AESprite* _host);
	VOID execute();
private:
	INT smokeCooldown = 0;
};


class HomingAI : public BulletAI {
public:
	HomingAI(AESprite* _host, AESprite* _target);
	VOID execute();
private:
	AESprite* target = nullptr;
	BOOLEAN isLocked = FALSE;
};


class TurretBaseAI : public AEAI {
public:
	TurretBaseAI(AESprite* _self);
	
	VOID execute();
private:

};


class TurretAI : public AEAI {
public:
	TurretAI(AESprite* _self);
	AESprite* getTarget() { return target; }
	VOID execute();
private:
	INT rocketCooldown = 0, homingFired = 0;;
	AESprite* target = nullptr;
};