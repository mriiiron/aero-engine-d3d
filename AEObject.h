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
#include "AEObjAnimation.h"


// Types of Objects.
typedef enum _ObjType {
	OBJ_UNKNOWN, OBJ_CHARACTER, OBJ_PROJECTILE
} ObjType;


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

	static const INT MAX_OBJECT_ACTIONS		= 100;

	VOID addAnimToSlot(INT slot, AEAnimation* _anim) { anim[slot] = _anim; }
	VOID setOid(INT _oid) { oid = _oid; }
	VOID setName(std::string _name) { name = _name; }
	VOID setType(INT _type) {
		switch (_type) {
		case 0: otype = OBJ_CHARACTER;  break;
		case 1: otype = OBJ_PROJECTILE;  break;
		default: break;
		}
	}
	AEAnimation* getAnim(INT index) { return anim[index]; }
	ObjType getType() { return otype; }
	std::string getName() { return name; }

private:

	INT oid;
	std::string name;
	AEAnimation* anim[MAX_OBJECT_ACTIONS];
	ObjType otype;

};


struct AERO_OBJECT_DESC {

	INT oid;
	std::string name;
	ObjType otype;

	AERO_OBJECT_DESC() {
		oid = 0;
		name = "Unknown Object";
		otype = OBJ_UNKNOWN;
	}

};


class AEObjectTable {

public:

	static const INT MAX_TOTAL_OBJECTS		= 100;

	VOID addAt(INT index, AEObject* obj) { table[index] = obj; }
	AEObject* get(INT index) { return table[index]; }

private:

	AEObject* table[MAX_TOTAL_OBJECTS];

};