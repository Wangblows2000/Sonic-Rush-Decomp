#include <ov000_c/stageobj.h>

typedef struct {
    u16 b0 : 5;
    u16 b5 : 11;
} UnkStruct_Bits5;

typedef struct {
    u8 pad0[0x20];
    u16 unk20;
    u8 pad22[2];
    UnkStruct_Bits5 unk24;
} UnkStruct_0208922c;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x452 - 0x20C];
    u16 unk452;
} UnkStruct_022f0fdc;

extern volatile UnkStruct_0208922c gDisplayMain;
extern UnkStruct_Bits5 data_02089250;

void BossFPartD8_Unk_ov000_022f0fdc(UnkStruct_022f0fdc *w, u16 n) {
    if (n >= ++w->unk452)
        return;
    w->unk452 = 0;
    if (gDisplayMain.unk24.b0 == 0)
        return;
    data_02089250.b0 -= 1;
    if (gDisplayMain.unk24.b0 == 0)
        gDisplayMain.unk20 &= ~0xc0;
}
