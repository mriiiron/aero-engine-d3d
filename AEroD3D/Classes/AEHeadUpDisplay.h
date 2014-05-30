/**
 * @file	AEHud.h
 * @brief	Defining the elements used by head-up displays.
 *
 * The HUDs is included in Scenes. Elements of HUD will not move along with ae_Camera movement.
 * You may create a relatively static scene only using HUD. For example, your game title screen.
 *
 * @author	CaiyZ (Mr.IroN)
 * @version	0.1
 * @date	2013
*/

#pragma once


struct AEHUDText {
	
	INT x, y;
	std::string text;

	AEHUDText(INT _x, INT _y, std::string _text) {
		x = _x;  y = _y;
		text = _text;
	}

};


struct AEHUDImage {

	INT x, y;
	AEResource* res;
	INT imgOffset;
	
	AEHUDImage(INT _x, INT _y, AEResource* _res, INT _imgOffset) {
		x = _x;  y = _y;
		res = _res;
		imgOffset = _imgOffset;
	}


};


class AEHeadUpDisplay {

public:

	static const INT MAX_TEXT_COUNT				= 30;
	static const INT MAX_IMAGE_COUNT			= 30;

	AEHeadUpDisplay();

	VOID addText(INT _x, INT _y, std::string _text);
	VOID addImage(INT _x, INT _y, AEResource* _res, INT _imgOffset);
	VOID update();
	VOID addToRenderBuffer();

private:

	AEHUDText* texts[MAX_TEXT_COUNT];
	AEHUDImage* images[MAX_IMAGE_COUNT];
	INT maxTextIndex, maxImageIndex;

};