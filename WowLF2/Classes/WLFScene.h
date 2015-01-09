#pragma once


class WLFShrineCaveScene : public AEScene {

public:

	static const FLOAT GRAVITY;

	WLFShrineCaveScene(INT spriteTableSize);

	VOID initialize();
	VOID update();
	VOID render(INT renderMode = RENDER_ALL);
	VOID processInput();
	VOID processCollision();

	VOID addNamepadToHUD(WLFCharacter* character, INT portraitIndex, INT slot);
	VOID removeNamepadFromHUD(WLFCharacter* character);

private:

	WLFWarrior* player;
	INT dik_left, dik_right, dik_up, dik_down, dik_attack_a, dik_attack_b, dik_jump, dik_special;

	static VOID loadObject(INT oid, std::string fileName, std::string objName, INT objType);

};