#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x466 - 0x20C];
    u16 unk466;
} UnkStruct_022e7b6c;

extern s16 data_0206e470[];

static inline s32 Mul64(s64 a, s64 b) {
    return (s32)((a * b + 0x800) >> 12);
}

void Boss7PartD5_Unk_ov000_022e7b6c(UnkStruct_022e7b6c *w) {
    w->base.unk2E = 0;
    w->base.unk14 += Mul64(data_0206e470[(u32)(((s32)(w->unk466 & 0xffff) >> 4) << 1)], 0x4cc) >> 4;
    w->unk466 += 0x100;
}
