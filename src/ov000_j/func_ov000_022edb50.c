#include <nitro/types.h>

typedef struct {
    u8 pad0[0x320];
    s32 unk320;
} UnkStruct_022edb50;

s32 func_0200225c(s32 a, s32 b);

void func_ov000_022edb50(UnkStruct_022edb50 *p, s32 a) {
    s32 v = func_0200225c(a << 12, 0x80000);

    p->unk320 = (s32)(((s64)v * 0x1000 + 0x800) >> 12) + 0x800;
}
