#pragma once


struct WLFCharacterHUDItem {
    INT namepadIndex, portraitIndex, barHPIndex, barEnergyIndex;
    INT buffIndex[20];
    INT buffCount;
    WLFCharacter* charRef;
};

class WLFHeadUpDisplay : public AEHeadUpDisplay {

public:

    WLFHeadUpDisplay(INT spriteTableSize);

    VOID update();

    VOID removeNamepad(INT slot);
    VOID setNamepad(INT slot, WLFCharacter* character);

private:

    WLFCharacterHUDItem items[2];

};