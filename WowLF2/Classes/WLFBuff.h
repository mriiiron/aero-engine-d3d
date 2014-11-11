#pragma once

#include <string>


class WLFCharacter;


class WLFBuff {

public:

	WLFBuff(INT _time);
	VOID setTimeRemain(INT _timeRemain) { timeRemain = _timeRemain; }
	VOID setHost(WLFCharacter* _host) { host = _host; }
	INT getTimeRemain() { return timeRemain; }
	BOOLEAN isDead() { return deadFlag; }

	virtual VOID update() = 0;

protected:

	WLFCharacter* host;

	std::string name;
	INT timeRemain;
	AEFrame* icon;
	BOOLEAN deadFlag;

};


class WLFBuffRend : public WLFBuff {

public:

	WLFBuffRend(INT _time);

	VOID update();

};