#include <nitro/types.h>

typedef struct UnkStruct_022f1d54_o {
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
} UnkStruct_022f1d54_o;

typedef struct UnkStruct_022f1d54_q {
    u8 pad0[0x74];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f1d54_o *unk84;
} UnkStruct_022f1d54_q;

typedef struct UnkStruct_022f1d54_s {
    u8 pad0[0x18];
    UnkStruct_022f1d54_q *unk18;
} UnkStruct_022f1d54_s;

typedef struct UnkStruct_022f1d54 {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    u8 pad20[0x50 - 0x20];
    s32 unk50;
    s32 unk54;
    u8 pad58[0x74 - 0x58];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f1d54_o *unk84;
    u8 pad88[0xb8 - 0x88];
    u32 unkB8;
    u8 padBC[0x160 - 0xbc];
    u32 unk160;
    u8 pad164[0x1c8 - 0x164];
    s32 unk1C8;
    s32 unk1CC;
    u8 pad1D0[0x1dc - 0x1d0];
    UnkStruct_022f1d54_s *unk1DC;
} UnkStruct_022f1d54;

extern void *data_ov001_0231f068;
extern void *data_ov001_0231f074;
extern u16 data_ov001_02316840[];
extern u32 data_0208fe70;

void func_02031a68(void *p, s32 a, void *b, s32 c, s32 d);

static inline u16 Rand(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return data_0208fe70 >> 16;
}

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

typedef struct UnkStruct_022f1d54_p {
    u8 pad0[0x18];
    s32 unk18;
    u8 pad1C[0x50 - 0x1c];
    s32 unk50;
    u8 pad54[0x74 - 0x54];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f1d54_o *unk84;
    u8 pad88[0x1dc - 0x88];
    UnkStruct_022f1d54_s *unk1DC;
} UnkStruct_022f1d54_p;

void func_ov001_022f1d24(void);

void func_ov001_022f1d54(UnkStruct_022f1d54_p *p) {
    UnkStruct_022f1d54_o *a = p->unk84;
    UnkStruct_022f1d54_o *b;

    if (p->unk18 + 0x5000 < 0x153000)
        return;
    b = p->unk1DC->unk18->unk84;
    p->unk18 = 0x153000;
    p->unk50 = 0;
    p->unk74 = func_ov001_022f1d24;
    a->unk10C |= 0x40;
    func_02031a68(a, 0, data_ov001_0231f068, data_ov001_02316840[0x2c], 0);
    func_02031a68(a, 4, data_ov001_0231f074, data_ov001_02316840[0x2d], 0);
    a->unk12C = 0x400;
    a->unk10C &= ~4;
    a->unk110 &= ~4;
    b->unk10C |= 0x40;
    func_02031a68(b, 0, data_ov001_0231f068, data_ov001_02316840[0x2e], 0);
    func_02031a68(b, 4, data_ov001_0231f074, data_ov001_02316840[0x2f], 0);
    b->unk12C = 0x400;
    b->unk10C &= ~4;
    b->unk110 &= ~4;
}
