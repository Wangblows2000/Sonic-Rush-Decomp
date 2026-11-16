#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0x438];
    u16 unk438;
} UnkStruct_Ctl;

typedef struct {
    u8 pad0[0x218];
    UnkStruct_Ctl *unk218;
} UnkStruct_02369b84;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x210 - 0x20C];
    u16 unk210;
    s16 unk212;
} UnkStruct_022f5ce4;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_02369b84 *data_ov000_02369b84;
extern s16 data_ov000_02363dc0[];
extern s16 data_ov000_02363dcc[];

void func_ov000_022f68c4(UnkStruct_022f5ce4 *w, s32 a, s32 b);

static inline BOOL IsMode23(void) {
    return data_022c4560.unk10 == 2 || data_022c4560.unk10 == 3;
}

static inline s32 GetLevel(void) {
    return IsMode23() ? 1 : data_022c4560.unk14;
}

void func_ov000_022f5ce4(UnkStruct_022f5ce4 *w) {
    s16 v;

    w->base.unk30 -= 0x40;
    w->unk210 += 0x800 - w->base.unk30;
    v = w->unk212;
    if (v < 0) {
        if (-v > 0x800)
            v = v + 0x800;
        else
            v = 0;
    } else if (v > 0) {
        if (v > 0x800)
            v = v - 0x800;
        else
            v = 0;
    }
    w->unk212 = v;
    func_ov000_022f68c4(w, data_ov000_02363dc0[GetLevel()], data_ov000_02363dcc[GetLevel()]);
    if (w->base.unk14 < 0x18000) {
        data_ov000_02369b84->unk218->unk438--;
        w->base.unk54 |= 4;
    }
}
