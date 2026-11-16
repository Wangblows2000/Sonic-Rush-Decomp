#include <nitro/types.h>

typedef struct UnkStruct_022fb974_s {
    u8 pad0[4];
    s32 unk4;
    u8 pad8[0x18 - 8];
    s32 x;
    s32 y;
    u8 pad20[4];
    s32 px;
    s32 py;
} UnkStruct_022fb974_s;

typedef struct UnkStruct_022fb974 {
    u8 pad0[0x50];
    s32 vx;
    s32 vy;
    u8 pad58[0x6c - 0x58];
    s16 unk6C;
    u8 pad6E[0x74 - 0x6e];
    void (*state)(struct UnkStruct_022fb974 *p);
    u8 pad78[0x9c - 0x78];
    s32 unk9C;
    void *unkA0;
    u8 padA4[0xe8 - 0xa4];
    u8 unkE8[4];
} UnkStruct_022fb974;

extern s16 data_0206e470[];

u16 func_020027a4(s32 y, s32 x);
void func_ov001_022fba78(UnkStruct_022fb974 *p);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov001_022fb974(UnkStruct_022fb974_s **self, UnkStruct_022fb974 **other) {
    UnkStruct_022fb974_s *s = *self;
    UnkStruct_022fb974 *p = *other;
    u16 a;

    if (s->unk4 != 0)
        return;
    p->unk9C = 0;
    p->unkA0 = p->unkE8;
    p->unk6C = 0;
    p->vx = p->vy = 0;
    a = func_020027a4(s->y - s->py, s->x - s->px);
    p->vx = FX_Mul(COS(a), 0x8000);
    p->vy = FX_Mul(SIN(a), 0x8000);
    p->state = func_ov001_022fba78;
    Sound_PlaySeqArc(0, 0, 0xa9);
}
