#pragma once

#include <string>


class WLFCharacter;


class WLFBuff {

public:

    WLFBuff(INT _time);
    VOID setTimeRemain(INT _timeRemain) { timeRemain = _timeRemain; }
    VOID setHost(WLFCharacter* _host) { host = _host; }
    VOID setCaster(WLFCharacter* _caster) { caster = _caster; }
    INT getTimeRemain() { return timeRemain; }
    INT getIconIndex() { return iconIndex; }
    WLFCharacter* getHost() { return host; }
    WLFCharacter* getCaster() { return caster; }
    std::string getName() { return name; }
    BOOLEAN isDead() { return deadFlag; }
    AESprite* getSprite(FLOAT cx, FLOAT cy);

    virtual VOID update() = 0;

protected:

    WLFCharacter* host;
    WLFCharacter* caster;

    std::string name;
    INT timeRemain;
    INT iconIndex;
    BOOLEAN deadFlag;

};


class WLFBuffRend : public WLFBuff {
public:
    WLFBuffRend(INT _time);
    VOID update();
};


class WLFBuffArmorCrack : public WLFBuff {

public:
    WLFBuffArmorCrack(INT _time);
    VOID update();
};


class WLFBuffSlowDown : public WLFBuff {
public:
    WLFBuffSlowDown(INT _time);
    VOID update();
};