#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x4AC - 0x20C];
    u16 unk4AC;
    u8 pad4AE[0x4B4 - 0x4AE];
    s32 unk4B4;
    s32 unk4B8;
    s16 unk4BC;
} UnkStruct_022f63cc;

extern UnkStruct_022c4560 data_022c4560;
extern s16 data_ov000_02363de4[];
extern s16 data_ov000_02363dc8[];

void func_ov000_022f62f0(UnkStruct_022f63cc *w);

static inline BOOL IsMode23(void) {
    return data_022c4560.unk10 == 2 || data_022c4560.unk10 == 3;
}

static inline s32 GetLevel(void) {
    return IsMode23() ? 1 : data_022c4560.unk14;
}

void func_ov000_022f63cc(UnkStruct_022f63cc *w) {
    w->unk4B4 = (((Rand() & 7) << 6) >> 3) + 0x4000;
    if (Rand() & 1)
        w->unk4B4 = -w->unk4B4;
    w->unk4B4 += w->base.unk10;
    w->unk4B8 = data_ov000_02363de4[GetLevel()];
    w->unk4BC = data_ov000_02363dc8[GetLevel()];
    w->unk4AC = 0;
    w->base.unk68 = func_ov000_022f62f0;
    ((void (*)(UnkStruct_022f63cc *))w->base.unk68)(w);
}
