#pragma once

#include <string>
#include <fstream>
#include <sstream>


struct WLFFromTo {
	INT from, to;
};


class WLFDataFileReader {

public:

	static AEResType resourceTypeDecrypt(std::string resTypeStr);
	static WLFFromTo framesFromToDecrypt(std::string framesFromTo);
	static INT moveInputDecrypt(std::string move);

	static VOID readResources(std::string fileName);
	static VOID readObject(std::string fileName, AEObject* obj);

private:



};