#include <nitro/types.h>

typedef struct UnkStruct_022f4d74_o {
    u8 pad0[0xe4];
    s32 *unkE4;
    u8 padE8[0x10c - 0xe8];
    u8 unk10C;
    u8 pad10D[3];
    u8 unk110;
    u8 pad111[3];
    s32 unk114;
    s32 unk118;
    u8 pad11C[0x12c - 0x11c];
    s16 unk12C;
} UnkStruct_022f4d74_o;

typedef struct UnkStruct_022f4d74_q {
    u8 pad0[0x74];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f4d74_o *unk84;
} UnkStruct_022f4d74_q;

typedef struct UnkStruct_022f4d74_s {
    u8 pad0[0x18];
    UnkStruct_022f4d74_q *unk18;
} UnkStruct_022f4d74_s;

typedef struct UnkStruct_022f4d74 {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    u8 pad20[0x50 - 0x20];
    s32 unk50;
    s32 unk54;
    u8 pad58[0x74 - 0x58];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f4d74_o *unk84;
    u8 pad88[0xb8 - 0x88];
    u32 unkB8;
    u8 padBC[0x160 - 0xbc];
    u32 unk160;
    u8 pad164[0x1c8 - 0x164];
    s32 unk1C8;
    s32 unk1CC;
    u8 pad1D0[0x1dc - 0x1d0];
    UnkStruct_022f4d74_s *unk1DC;
} UnkStruct_022f4d74;

extern void *data_ov001_0231f068;
extern void *data_ov001_0231f074;
extern u16 data_ov001_02316840[];
extern u32 gRandSeed;

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return gRandSeed >> 16;
}

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

typedef struct UnkStruct_022f4d74_s2 {
    u8 pad0[0x38];
    s32 unk38;
    s32 unk3C;
    s32 unk40;
} UnkStruct_022f4d74_s2;

typedef struct UnkStruct_022f4d74_p {
    u8 pad0[0x74];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f4d74_o *unk84;
    u8 pad88[0x1c8 - 0x88];
    s32 unk1C8;
    s32 unk1CC;
    u8 pad1D0[4];
    u16 unk1D4;
    u8 pad1D6[0x1dc - 0x1d6];
    UnkStruct_022f4d74_s2 *unk1DC;
} UnkStruct_022f4d74_p;

extern s32 data_ov001_0231f520;
extern s32 data_ov001_0231f524;

void func_ov001_022f4d04(void);
void func_ov001_022fad38(void *parent, s32 x, s32 y, s32 z, u16 ang);

void func_ov001_022f4d74(UnkStruct_022f4d74_p *p) {
    UnkStruct_022f4d74_o *a = p->unk84;
    UnkStruct_022f4d74_s2 *s;

    if (!(a->unk10C & 2))
        return;
    p->unk1CC--;
    if (p->unk1CC <= 0) {
        p->unk74 = func_ov001_022f4d04;
        Model_SetAnimation(a, 0, data_ov001_0231f068, 0x16, 0);
        a->unk10C |= 4;
        p->unk1C8 = 0xf0;
        return;
    }
    s = p->unk1DC;
    Model_SetAnimation(a, 0, data_ov001_0231f068, 0x15, 0);
    func_ov001_022fad38(p, s->unk38 - data_ov001_0231f524, -(s->unk3C + data_ov001_0231f520), s->unk40, (p->unk1CC - 1) * p->unk1D4 + 0x9555);
}
