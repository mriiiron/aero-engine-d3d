/**
 * @file	AEAI.h
 * @brief	Includes the definations of AI.
 *
 * The AI in AEro Engine is just like a "soul" to the sprite. Imagine that when a 
 * sprite losts its AI, it would has no perception about its surroundings, thus
 * it would just act as a walking dead.
 *
 * Q: What kinds of Sprite need an AI?
 * A: Enemy fighters, automatic turrets, tracking missiles, etc.
 *
 * Q: For what AI is unnecessary?
 * A: Bullets, energy balls, falling leaves, etc.
 *
 * @author	CaiyZ (Mr.IroN)
 * @version	0.1
 * @date	2013
**/

#pragma once


class AEAI {

public:

	static const INT EXEC_INTERVAL			= 3;

	AEAI(AESprite* _self);
	VOID releaseAllKeys();
	VOID setTarget(AESprite* _target) { target = _target; }
	VOID enable() { enabled = TRUE; }
	VOID disable() { enabled = FALSE; }
	VOID setIndex(INT _index) { index = _index; }
	BOOLEAN isEnabled() { return (enabled ? 1 : 0); }
	INT checkdistXWith(AESprite* sp) { return fabs(self->getCx() - sp->getCx()); }
	INT checkdistYWith(AESprite* sp) { return fabs(self->getCy() - sp->getCy()); }

	virtual VOID execute() = 0;

protected:

	AESprite* self;
	AESprite* target;
	BOOLEAN enabled;
	INT index;

};

class AEAITable {

public:

	static const INT MAX_ONLINE_AIS = 120;

	AEAITable();
	AEAI* get(INT index) { return table[index]; }
	AEAI* getByHash(INT hashIndex) { return table[hash[hashIndex]]; }
	INT getHashCount() { return pHash; }
	VOID add(AEAI* ai);
	VOID addAt(INT index, AEAI* sp);
	VOID remove(INT index);
	VOID clear();
	VOID update();

private:

	AEAI* table[MAX_ONLINE_AIS];
	INT occupied[MAX_ONLINE_AIS];
	INT hash[MAX_ONLINE_AIS];
	INT maxIndex, pHash;

};