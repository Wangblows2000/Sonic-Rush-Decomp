#include <nitro/types.h>

typedef struct UnkStruct_022efcdc_o {
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
} UnkStruct_022efcdc_o;

typedef struct UnkStruct_022efcdc_q {
    u8 pad0[0x74];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022efcdc_o *unk84;
} UnkStruct_022efcdc_q;

typedef struct UnkStruct_022efcdc_s {
    u8 pad0[0x18];
    UnkStruct_022efcdc_q *unk18;
} UnkStruct_022efcdc_s;

typedef struct UnkStruct_022efcdc {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    u8 pad20[0x50 - 0x20];
    s32 unk50;
    s32 unk54;
    u8 pad58[0x74 - 0x58];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022efcdc_o *unk84;
    u8 pad88[0xb8 - 0x88];
    u32 unkB8;
    u8 padBC[0x160 - 0xbc];
    u32 unk160;
    u8 pad164[0x1c8 - 0x164];
    s32 unk1C8;
    s32 unk1CC;
    u8 pad1D0[0x1dc - 0x1d0];
    UnkStruct_022efcdc_s *unk1DC;
} UnkStruct_022efcdc;

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

typedef struct UnkStruct_022efcdc_s2 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 padC[0x18 - 0xc];
    UnkStruct_022efcdc_q *unk18;
} UnkStruct_022efcdc_s2;

typedef struct UnkStruct_022efcdc_p {
    u8 pad0[0x50];
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    u8 pad64[0x74 - 0x64];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022efcdc_o *unk84;
    u8 pad88[0x1c8 - 0x88];
    s16 unk1C8;
    u8 pad1CA[2];
    s16 unk1CC;
    u8 pad1CE[0x1dc - 0x1ce];
    UnkStruct_022efcdc_s2 *unk1DC;
} UnkStruct_022efcdc_p;

void func_ov001_022efa00(void);

void func_ov001_022efcdc(UnkStruct_022efcdc_p *p) {
    UnkStruct_022efcdc_s2 *s = p->unk1DC;
    UnkStruct_022efcdc_o *b;
    UnkStruct_022efcdc_o *a;

    a = p->unk84;
    b = s->unk18->unk84;

    s->unk8 = s->unk0;
    s->unk0 = 2;
    p->unk74 = func_ov001_022efa00;
    a->unk10C |= 0x40;
    func_02031a68(a, 0, data_ov001_0231f068, data_ov001_02316840[4], 0);
    func_02031a68(a, 4, data_ov001_0231f074, data_ov001_02316840[5], 0);
    a->unk12C = 0x400;
    a->unk10C &= ~4;
    a->unk110 &= ~4;
    a->unk114 = 0x1000;
    b->unk10C |= 0x40;
    func_02031a68(b, 0, data_ov001_0231f068, data_ov001_02316840[6], 0);
    func_02031a68(b, 4, data_ov001_0231f074, data_ov001_02316840[7], 0);
    b->unk12C = 0x400;
    b->unk10C &= ~4;
    b->unk110 &= ~4;
    b->unk114 = 0x1000;
    p->unk50 = 0x6000;
    p->unk60 = 0;
    p->unk5C = p->unk60;
    p->unk54 = p->unk5C;
    p->unk1C8 = 0;
    p->unk1CC = 0;
}
