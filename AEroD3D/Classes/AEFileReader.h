#pragma once

#include <string>
#include <fstream>
#include <sstream>


struct AEFromTo {
    INT from, to;
};


class AEDataFileReader {

public:

    static AEResType resourceTypeDecrypt(std::string resTypeStr);
    static AEFromTo framesFromToDecrypt(std::string framesFromTo);

    static VOID loadResources(std::string fileName);
    static VOID loadObject(INT oid, std::string fileName);
    static VOID loadBackground(INT bgid, std::string fileName);

private:



};