#include <nitro/types.h>

typedef struct UnkStruct_022faf00_c {
    u8 pad0[8];
    u32 unk8;
} UnkStruct_022faf00_c;

typedef struct UnkStruct_022faf00_m {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
} UnkStruct_022faf00_m;

typedef struct UnkStruct_022faf00_a {
    u8 pad0[4];
    s32 unk4;
    u8 pad8[0x18 - 8];
    s32 unk18;
    s32 unk1C;
    u8 pad20[4];
    s32 unk24;
    s32 unk28;
} UnkStruct_022faf00_a;

typedef struct UnkStruct_022faf00_t {
    u8 pad0[0x40];
    u16 unk40;
    u8 pad42[0x50 - 0x42];
    s32 unk50;
    s32 unk54;
    u8 pad58[0x74 - 0x58];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022faf00_m *unk84;
    u8 pad88[0x98 - 0x88];
    s16 unk98;
    u8 pad9A[2];
    UnkStruct_022faf00_c *unk9C;
    UnkStruct_022faf00_c *unkA0;
    void *unkA4;
    u8 padA8[0x120 - 0xa8];
    u8 unk120[0x38];
} UnkStruct_022faf00_t;

extern s16 data_0206e470[];

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void ExtraBoss_InitCollider(void *a, void *b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);
u16 FX_Atan2(s32 y, s32 x);
void ExtraBossObj_State_Unk_ov001_022fd1ec(void);
void Sound_PlaySeqArc(void *p, u32 a, u32 b);

void ExtraBoss_Unk_ov001_022faf00(UnkStruct_022faf00_a **pa, UnkStruct_022faf00_t **pt) {
    UnkStruct_022faf00_a *a = *pa;
    UnkStruct_022faf00_t *t = *pt;
    u16 ang;

    if (a->unk4 != 0)
        return;
    t->unk9C->unk8 |= 0x200;
    t->unkA0->unk8 |= 0x200;
    ExtraBoss_InitCollider(t->unk120, t, 4, 1, 0x10000, 0x10000, 0x10000, 0, 0, 0, 0);
    t->unkA4 = t->unk120;
    t->unk98 = 1;
    t->unk84->unk18 = 0x6998;
    t->unk84->unk1C = 0x34cc;
    ang = FX_Atan2(a->unk1C - a->unk28, a->unk18 - a->unk24);
    t->unk50 = FX_Mul(COS(ang), 0x8000);
    t->unk54 = FX_Mul(SIN(ang), 0x8000);
    t->unk40 = 0x10000 - ang;
    t->unk74 = ExtraBossObj_State_Unk_ov001_022fd1ec;
    Sound_PlaySeqArc(0, 0, 0xa9);
}
