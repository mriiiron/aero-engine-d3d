#pragma once


class WLFFloatingTextAI : public AEAI {

public:

	WLFFloatingTextAI(AESprite* _host);
	VOID execute();

private:

	INT time;
	FLOAT floatSpeed;

};