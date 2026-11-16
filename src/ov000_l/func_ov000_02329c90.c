#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;

s16 func_02068df8(s32 v, s32 a, s32 b);
s32 func_02066a9c(s32 x, s32 y, s32 a, s32 b, s32 c, s32 d);

void func_ov000_02329c90(UnkStruct_EffObj *w) {
    s32 r;

    w->unk30 = func_02068df8(w->unk30, -13, w->unk8);
    w->unk2E = w->unk30 >> 1;
    if (data_022c4560.unk20 & 2)
        w->unk2E = -w->unk2E;
    r = func_02066a9c(w->unk10 >> 8, w->unk14 >> 8, 1, 2, 0, 0);
    if (w->unkC > (w->unk14 >> 8) || r < 0)
        w->unk54 |= 4;
}
