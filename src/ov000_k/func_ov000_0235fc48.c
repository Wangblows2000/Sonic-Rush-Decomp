#include <nitro/types.h>

typedef struct {
    u8 pad0[0xFD8];
    u32 unkFD8[7];
} UnkStruct_0235fc48;

typedef struct {
    u8 pad0[0x14];
    u16 unk14;
} UnkStruct_020892ac;

extern u32 gObjVramBaseTable;
extern UnkStruct_020892ac data_020892ac;

u16 *Oam_AllocObj(s32 a, s32 b);

#pragma thumb on
void func_ov000_0235fc48(u8 *w, s32 x0, s32 y0, s32 idx) {
    UnkStruct_0235fc48 *p;
    s32 i;
    s32 y;
    u32 base;
    u8 yy;
    u32 v;
    u16 *oam;
    s32 x;

    x = x0 + 0x14;
    y = y0 + 0xb;
    i = 0;
    p = (UnkStruct_0235fc48 *)(w + idx * 0x1c);
    base = gObjVramBaseTable;
    yy = y;
    for (; i < 7; i++, x += 8) {
        if (x >= -8 && x < 0x100 && y >= -8 && y < 0xc0) {
            v = (p->unkFD8[0] - base) >> (data_020892ac.unk14 + 5);
            oam = Oam_AllocObj(0, 0);
            oam[0] = yy;
            oam[1] = (u8)x;
            oam[2] = (v & 0x3ff) | 0xc000;
        }
        p = (UnkStruct_0235fc48 *)((u8 *)p + 4);
    }
}
#pragma thumb reset
