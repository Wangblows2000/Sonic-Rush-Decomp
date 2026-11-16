#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u16 unk4;
} UnkStruct_023016c4_1e0;

typedef struct {
    u8 pad0[0x30];
    s16 unk30;
    u8 pad32[0x1e0 - 0x32];
    UnkStruct_023016c4_1e0 *unk1E0;
    u8 pad1E4[0x238 - 0x1e4];
    u32 unk238;
} UnkStruct_023016c4;

s16 func_02068db4(s32 a, s32 b);
void func_ov000_0230187c(UnkStruct_023016c4 *p);

void func_ov000_023016c4(UnkStruct_023016c4 *p) {
    s16 spd;
    s32 v;

    spd = 0x100;
    v = p->unk1E0->unk4 & 0xc;
    if (v)
        spd += (v >> 2) << 7;
    p->unk30 = func_02068db4(p->unk30, (spd << 5) >> 8);
    p->unk238 ^= 0x10;
    p->unk238 |= 0x10000;
    func_ov000_0230187c(p);
}
