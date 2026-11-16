#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad[0x20];
    u32 unk20;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;

s32 func_02068ce4(s32 cur, s32 target, s32 a, s32 b, s32 step);

void func_ov000_0230023c(UnkStruct_StageObj *w) {
    if ((data_022c4560.unk20 & 3) == 0) {
        w->unkC = Rand() & 0x1ff;
        if (Rand() & 1)
            w->unkC = -w->unkC;
        if (w->unk14 > (w->unk1EC + 8) << 8 && w->unk30 > 0)
            w->unk30 = -0x1ff;
        if (w->unk14 < (w->unk1EC - 8) << 8 && w->unk30 < 0)
            w->unk30 = 0x1ff;
    }
    w->unk30 = func_02068ce4(w->unk30, w->unkC, 1, 0, 0x40);
}
