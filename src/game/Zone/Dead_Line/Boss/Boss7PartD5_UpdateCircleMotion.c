#include <game/Stage/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x224 - 0x20C];
    s32 unk224;
} UnkStruct_Boss;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x466 - 0x20C];
    u16 unk466;
    s16 unk468;
    u8 pad46A[2];
    s32 unk46C;
} UnkStruct_022e8538;

extern UnkStruct_Boss *gBoss7;
extern s16 data_0206e470[];

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void Boss7PartD5_UpdateCircleMotion(UnkStruct_022e8538 *w) {
    UnkStruct_Boss *b = gBoss7;
    s32 i;

    w->unk466 += w->unk468;
    i = (s32)(w->unk466 & 0xffff) >> 4;
    w->base.unk10 = FX_Mul(data_0206e470[(u32)((i << 1) + 1)], w->unk46C) >> 4;
    w->base.unk10 += b->unk224;
    i = (s32)(w->unk466 & 0xffff) >> 4;
    w->base.unk18 = FX_Mul(data_0206e470[(u32)(i << 1)], w->unk46C) >> 4;
}
