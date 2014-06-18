/**
 * @file	AEObject.h
 * @brief	Defining the basic Objects used in our games.
 *
 * @author	CaiyZ (Mr.IroN)
 * @version	0.1
 * @date	2013
*/

#pragma once


#include <string>
#include "AEAnimation.h"


// Types of Objects.
enum AEObjType {
	OBJ_UNKNOWN, OBJ_CHARACTER, OBJ_PROJECTILE, OBJ_EFFECT
};


struct AERO_OBJECT_DESC {

	INT oid;
	std::string name;
	AEObjType otype;

	AERO_OBJECT_DESC() {
		oid = 0;
		name = "Unknown Object";
		otype = OBJ_UNKNOWN;
	}

};


// OBJECT: a complex union consists of various kinds of data:
// - OID: Every object holds an unique ID, so they could be easily referred
//   by the Sprites. OID is just the index number they occupies in the Object
//   Table.
// - ANIMATION: An object holds several animation arrays, each of them could
//   be called as "actions". These actions are defined in the certain object's
//   "data.txt". An active Sprite on the screen can freely change its action
//   from one to another. This happens when this Sprite moves, jumps, takes
//   damage, etc.
// - OTYPE: Different kinds of object must be differently dealed with. e.g.
//   when hitting a guy, an energy ball would explode, but a character would
//   not.
class AEObject {

public:

	static const INT MAX_ANIM_COUNT			= 100;

	AEObject(AERO_OBJECT_DESC desc);

	AEAnimation* getAnim(INT index) { return animTable[index]; }
	AEObjType getType() { return otype; }
	std::string getName() { return name; }

	VOID addAnim(INT slot, AEAnimation* _anim);

private:

	INT oid;
	std::string name;
	AEAnimation* animTable[MAX_ANIM_COUNT];
	AEObjType otype;

};