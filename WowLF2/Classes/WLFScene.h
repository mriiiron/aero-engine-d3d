#pragma once


class WLFShrineCaveScene : public AEScene {

public:

	WLFShrineCaveScene(AEBackground* _bg, AEHashedTable<AEPlatform>* _platformTable, AEHashedTable<AESprite>* _spriteTable, AEHeadUpDisplay* _hud);

	static const FLOAT GRAVITY;

	VOID initialize();
	VOID update();
	VOID render();
	VOID processInput();

	VOID processCollision();

private:

	WLFCharacter* player;

	static VOID addObject(INT oid, std::string fileName, std::string objName, AEObjType objType);

};