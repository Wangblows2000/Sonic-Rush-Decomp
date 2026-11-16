#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    u8 pad18[0x330 - 0x18];
    u32 unk330;
} UnkObj_02342e28;

extern UnkObj_02342e28 *volatile data_ov000_0236ac60[];
typedef struct {
    u8 pad0[0x54];
    u32 unk54;
    u8 pad58[0x32c - 0x58];
    u32 unk32C;
    u8 pad330[0x408 - 0x330];
    s32 unk408;
    s32 unk40C;
} UnkStruct_02342e28;

void func_ov000_02348e90(UnkStruct_02342e28 *w);

void func_ov000_02342e28(UnkStruct_02342e28 *w) {
    if (w->unk32C & 0x1000000)
        return;
    if (data_ov000_0236ac60[1]->unk330 & 0x800000)
        w->unk54 |= 1;
    else
        w->unk54 &= ~1;
    w->unk408 = data_ov000_0236ac60[1]->x;
    w->unk40C = data_ov000_0236ac60[1]->y;
    func_ov000_02348e90(w);
}
