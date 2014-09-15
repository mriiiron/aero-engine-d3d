#pragma once

#include <string>
#include <fstream>
#include <sstream>


class WLFDataFileReader {

public:

	static AEResType resourceTypeDecrypt(std::string resTypeStr);

	static VOID readResources(std::string fileName, AEConstantTable<AEResource>* pResourceTable, ID3D11Device** device);
	static VOID readObject(std::string fileName, AEObject* obj);

private:



};