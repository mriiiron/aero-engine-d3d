/**
 * @file	AEHud.h
 * @brief	Defining the elements used by head-up displays.
 *
 * The HUDs is included in Scenes. Elements of HUD will not move along with camera movement.
 * You may create a relatively static scene only using HUD. For example, your game title screen.
 *
 * @author	CaiyZ (Mr.IroN)
 * @version	0.1
 * @date	2013
*/

#pragma once


typedef struct _HUDText {
	std::string text;
	INT x, y;
} HUDText;


typedef struct _HUDImage {
	AEResource* res;
	INT imgOffset;
	INT x, y;
} HUDImage;


class AEHeadUpDisplay {

public:

	static const INT MAX_TEXT_COUNT				= 30;
	static const INT MAX_IMAGE_COUNT			= 30;

	AEHeadUpDisplay();
	VOID addText(INT _x, INT _y, std::string text);
	VOID addImage(INT _x, INT _y, AEResource* _res, INT _imgOffset);
	VOID update();
	VOID paint();

private:

	HUDText* texts[MAX_TEXT_COUNT];
	HUDImage* images[MAX_IMAGE_COUNT];
	INT maxTextIndex, maxImageIndex;

};