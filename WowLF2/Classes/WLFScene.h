#pragma once


class WLFShrineCaveScene : public AEScene {

public:

	WLFShrineCaveScene(INT spriteTableSize);

	static const FLOAT GRAVITY;

	VOID initialize();
	VOID update();
	VOID render(INT renderMode = RENDER_ALL);
	VOID processInput();

	VOID processCollision();

private:

	WLFCharacter* player;

	static VOID addObject(INT oid, std::string fileName, std::string objName, AEObjType objType);

};