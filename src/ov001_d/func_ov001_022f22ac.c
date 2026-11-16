#include <nitro/types.h>

typedef struct UnkStruct_022f22ac_o {
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
} UnkStruct_022f22ac_o;

typedef struct UnkStruct_022f22ac_q {
    u8 pad0[0x74];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f22ac_o *unk84;
} UnkStruct_022f22ac_q;

typedef struct UnkStruct_022f22ac_s {
    u8 pad0[0x18];
    UnkStruct_022f22ac_q *unk18;
} UnkStruct_022f22ac_s;

typedef struct UnkStruct_022f22ac {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    u8 pad20[0x50 - 0x20];
    s32 unk50;
    s32 unk54;
    u8 pad58[0x74 - 0x58];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f22ac_o *unk84;
    u8 pad88[0xb8 - 0x88];
    u32 unkB8;
    u8 padBC[0x160 - 0xbc];
    u32 unk160;
    u8 pad164[0x1c8 - 0x164];
    s32 unk1C8;
    s32 unk1CC;
    u8 pad1D0[0x1dc - 0x1d0];
    UnkStruct_022f22ac_s *unk1DC;
} UnkStruct_022f22ac;

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

typedef struct UnkStruct_022f22ac_p {
    u8 pad0[0x18];
    s32 unk18;
    u8 pad1C[0x74 - 0x1c];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f22ac_o *unk84;
    u8 pad88[0x1cc - 0x88];
    s32 unk1CC;
    s16 unk1D0;
    u8 pad1D2[2];
    s16 unk1D4;
    u8 pad1D6[0x1dc - 0x1d6];
    UnkStruct_022f22ac_s *unk1DC;
} UnkStruct_022f22ac_p;

void func_ov001_022f2138(void);

void func_ov001_022f22ac(UnkStruct_022f22ac_p *p) {
    UnkStruct_022f22ac_o *a = p->unk84;
    UnkStruct_022f22ac_o *b;
    s32 v;
    s32 acc;
    s32 i;

    p->unk1D0 += p->unk1D4;
    if (p->unk1D0 > 0x1000)
        p->unk1D0 = 0x1000;
    i = 2;
    v = p->unk1D0;
    acc = p->unk1CC;
    do {
        acc += FX_Mul(0xc8000 - acc, v);
    } while (i-- != 0);
    p->unk18 = acc;
    if (!(a->unk10C & 2))
        return;
    b = p->unk1DC->unk18->unk84;
    p->unk74 = func_ov001_022f2138;
    func_02031a68(a, 0, data_ov001_0231f068, data_ov001_02316840[0x1c], 0);
    func_02031a68(a, 4, data_ov001_0231f074, data_ov001_02316840[0x1d], 0);
    a->unk114 = 0x1000;
    func_02031a68(b, 0, data_ov001_0231f068, data_ov001_02316840[0x1e], 0);
    func_02031a68(b, 4, data_ov001_0231f074, data_ov001_02316840[0x1f], 0);
    b->unk114 = 0x1000;
}
