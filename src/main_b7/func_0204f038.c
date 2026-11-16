#include <nitro/types.h>

typedef struct {
    u8 pad0[0x34];
    u32 idx;
    u8 pad38[0x358 - 0x38];
    s32 split;
    s32 dist;
} UnkObj_0204f038;

extern u32 gSystemFlags;
u16 *func_0204ef90(UnkObj_0204f038 *o);
s32 func_0204eedc(s32 from, s32 to, s32 len, s32 cur, s32 e);

#pragma thumb on
void func_0204f038(UnkObj_0204f038 *o, s32 cur, s32 len, s32 e) {
    u16 *buf = func_0204ef90(o);
    s32 d = o->dist;
    u16 a = func_0204eedc(-d, 0, len, cur, e);
    u16 b = func_0204eedc(d, 0, len, cur, e);
    s32 i;
    for (i = 0; i < o->split; i++)
        buf[i] = a;
    for (; i < 0xc0; i++)
        buf[i] = b;
    gSystemFlags |= 0x1000 << o->idx;
}
#pragma thumb reset
