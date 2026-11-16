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
    u8 pad20C[0x438 - 0x20C];
    s32 unk438;
    s16 unk43C;
    u8 pad43E[0x454 - 0x43E];
    u16 unk454;
} UnkStruct_022efd08;

extern volatile UnkStruct_0208922c data_0208922c;
extern UnkStruct_Bits5 data_02089250;
extern UnkStruct_StageObj *data_ov000_0236ac60;

void func_ov000_022efd08(UnkStruct_022efd08 *w) {
    if (data_0208922c.unk24.b0 != 0 && !(w->unk454++ & 1)) {
        data_02089250.b0 -= 1;
        if (data_0208922c.unk24.b0 == 0)
            data_0208922c.unk20 &= ~0xc0;
    }
    if (w->unk43C > 0)
        w->unk43C -= 0x60;
    if (w->unk43C < 0)
        w->unk43C = 0;
    w->unk438 += w->unk43C;
    data_ov000_0236ac60->unk18 = w->unk438;
    w->base.unk32 -= 0x60;
    if (w->base.unk32 < 0)
        w->base.unk32 = 0;
}
