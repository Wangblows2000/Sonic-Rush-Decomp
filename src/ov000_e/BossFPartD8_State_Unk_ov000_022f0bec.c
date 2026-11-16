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
    u8 pad20C[0x454 - 0x20C];
    u16 unk454;
} UnkStruct_022f0bec;

extern volatile UnkStruct_0208922c gDisplayMain;
extern UnkStruct_Bits5 data_02089250;

void BossFPartD8_State_Unk_ov000_022f0bd0(void);

void BossFPartD8_State_Unk_ov000_022f0bec(UnkStruct_022f0bec *w) {
    w->unk454++;
    if (w->unk454 <= 6)
        return;
    w->unk454 = 0;
    data_02089250.b0 -= 1;
    if (gDisplayMain.unk24.b0 == 0) {
        gDisplayMain.unk20 &= ~0xc0;
        w->base.unk68 = BossFPartD8_State_Unk_ov000_022f0bd0;
    }
}
