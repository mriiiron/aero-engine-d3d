#include <d3d11_1.h>
#include <string>
#include "AEGameControl.h"

VOID AENSGameControl::exitGame(std::string message) {
	MessageBox(
		nullptr,
		"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.",
		"Error",
		MB_OK
	);
}