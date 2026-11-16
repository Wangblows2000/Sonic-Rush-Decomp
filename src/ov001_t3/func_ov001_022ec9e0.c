#include <nitro/types.h>

typedef struct {
    u8 pad0[0x11a0];
    u32 vals[2][7];
} UnkStruct_022ec9e0;

typedef struct {
    u8 pad0[0x14];
    u16 unk14;
} UnkStruct_020892ac;

extern const u32 data_02072c70;
extern UnkStruct_020892ac data_020892ac;

u16 *func_02036bfc(u32 idx, u32 prio);

#pragma thumb on
void func_ov001_022ec9e0(UnkStruct_022ec9e0 *p, s32 x, s32 y, s32 which) {
    s32 pal;
    s32 prio;
    s32 i;
    u32 val;
    u16 *o;

    pal = which == 0 ? 0xc : 0xd;
    prio = which == 0 ? 1 : 2;
    for (i = 0; i < 7; i++, x += 8) {
        if (x >= -8 && x < 0x100 && y >= -8 && y < 0xc0) {
            val = (p->vals[which][i] - data_02072c70) >> (data_020892ac.unk14 + 5);
            o = func_02036bfc(0, 0);
            o[0] = (u8)y;
            o[1] = (u8)x;
            o[2] = (pal << 12) | (val & 0x3ff) | ((prio << 10) & 0xc00);
        }
    }
}
#pragma thumb reset
