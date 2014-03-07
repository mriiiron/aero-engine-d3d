#include <d3d11_1.h>
#include <string>
#include "AEResource.h"
#include "AEHeadUpDisplay.h"

AEHeadUpDisplay::AEHeadUpDisplay() {
	maxTextIndex = maxImageIndex = 0;
}

VOID AEHeadUpDisplay::addText(INT _x, INT _y, std::string _text) {
	texts[maxTextIndex] = new AEHUDText(_x, _y, _text);
	maxTextIndex++;
}

VOID AEHeadUpDisplay::addImage(INT _x, INT _y, AEResource* _res, INT _imgOffset) {
	images[maxImageIndex] = new AEHUDImage(_x, _y, _res, _imgOffset);
	maxImageIndex++;
}

VOID AEHeadUpDisplay::update() {

}

VOID AEHeadUpDisplay::addToRenderBuffer() {
	for (INT i = 0; i < maxTextIndex; i++) {
		// TODO: Paint Texts
	}
	for (INT i = 0; i < maxImageIndex; i++) {
		// TODO: Paint Images
	}
}