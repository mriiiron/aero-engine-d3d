#pragma once


class WLFShrineCaveScene : public AEScene {

public:

	static const INT NAMEPAD_SLOT_PLAYER = 0;
	static const INT NAMEPAD_SLOT_TARGET = 1;

	WLFShrineCaveScene(INT spriteTableSize);

	static const FLOAT GRAVITY;

	VOID initialize();
	VOID update();
	VOID render(INT renderMode = RENDER_ALL);
	VOID processInput();

	VOID processCollision();

	VOID addNamepadToHUD(WLFCharacter* character, INT portraitIndex, INT slot);

private:

	WLFCharacter* player;

	static VOID loadObject(INT oid, std::string fileName, std::string objName, AEObjType objType);
	

};