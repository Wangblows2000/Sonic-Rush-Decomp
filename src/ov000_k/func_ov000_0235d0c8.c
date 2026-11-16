#include <nitro/types.h>

typedef struct {
    u8 pad0[0xDDC];
    u32 unkDDC[7];
} UnkStruct_0235d0c8;

typedef struct {
    u8 pad0[0x14];
    u16 unk14;
} UnkStruct_020892ac;

extern u32 data_02072c70;
extern UnkStruct_020892ac data_020892ac;

u16 *func_02036bfc(s32 a, s32 b);

#pragma thumb on
void func_ov000_0235d0c8(u8 *w, s32 x, s32 y, s32 idx) {
    UnkStruct_0235d0c8 *p;
    s32 i;
    u32 base;
    u8 yy;
    u32 v;
    u16 *oam;

    i = 0;
    p = (UnkStruct_0235d0c8 *)(w + idx * 0x1c);
    base = data_02072c70;
    yy = y;
    for (; i < 7; i++, x += 8) {
        if (x >= -8 && x < 0x100 && y >= -8 && y < 0xc0) {
            v = (p->unkDDC[0] - base) >> (data_020892ac.unk14 + 5);
            oam = func_02036bfc(0, 0);
            oam[0] = yy;
            oam[1] = (u8)x;
            oam[2] = (v & 0x3ff) | 0xc000;
        }
        p = (UnkStruct_0235d0c8 *)((u8 *)p + 4);
    }
}
#pragma thumb reset
