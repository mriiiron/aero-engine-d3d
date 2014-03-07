#include <d3d11_1.h>
#include <string>
#include "AEGameControl.h"

VOID AENSGameControl::exitGame(std::string message) {
	MessageBox(nullptr, message.c_str(), "Error", MB_OK);
	exit(EXIT_FAILURE);
}