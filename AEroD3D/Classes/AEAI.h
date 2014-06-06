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


// Forward Declartion
class AESprite;


class AEAI {

public:

	static const INT EXEC_INTERVAL			= 3;

	AEAI(AESprite* _host);
	VOID enable() { enabled = TRUE; }
	VOID disable() { enabled = FALSE; }
	VOID setIndex(INT _index) { index = _index; }
	BOOLEAN isEnabled() { return (enabled ? 1 : 0); }

	virtual VOID execute() = 0;

protected:

	AESprite* host;
	BOOLEAN enabled;
	INT index;

};