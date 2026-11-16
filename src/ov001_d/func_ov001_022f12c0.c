#include <nitro/types.h>

typedef struct UnkStruct_022f12c0_o {
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
} UnkStruct_022f12c0_o;

typedef struct UnkStruct_022f12c0_q {
    u8 pad0[0x74];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f12c0_o *unk84;
} UnkStruct_022f12c0_q;

typedef struct UnkStruct_022f12c0_s {
    u8 pad0[0x18];
    UnkStruct_022f12c0_q *unk18;
} UnkStruct_022f12c0_s;

typedef struct UnkStruct_022f12c0 {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    u8 pad20[0x50 - 0x20];
    s32 unk50;
    s32 unk54;
    u8 pad58[0x74 - 0x58];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f12c0_o *unk84;
    u8 pad88[0xb8 - 0x88];
    u32 unkB8;
    u8 padBC[0x160 - 0xbc];
    u32 unk160;
    u8 pad164[0x1c8 - 0x164];
    s32 unk1C8;
    s32 unk1CC;
    u8 pad1D0[0x1dc - 0x1d0];
    UnkStruct_022f12c0_s *unk1DC;
} UnkStruct_022f12c0;

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

void func_ov001_022f1130(void);
void func_ov001_022f10d0(void);

void func_ov001_022f12c0(UnkStruct_022f12c0 *p) {
    UnkStruct_022f12c0_o *a = p->unk84;
    UnkStruct_022f12c0_o *b = p->unk1DC->unk18->unk84;

    if (*a->unkE4 > 0xb4000) {
        p->unk1CC = -0x4000;
        p->unk74 = func_ov001_022f1130;
        return;
    }
    if (*a->unkE4 < 0xb4000) {
        p->unk1CC = 0x4000;
        p->unk74 = func_ov001_022f1130;
        return;
    }
    func_02031a68(a, 0, data_ov001_0231f068, data_ov001_02316840[0x30], 0);
    func_02031a68(a, 4, data_ov001_0231f074, data_ov001_02316840[0x31], 0);
    a->unk10C &= ~4;
    a->unk110 &= ~4;
    func_02031a68(b, 0, data_ov001_0231f068, data_ov001_02316840[0x32], 0);
    func_02031a68(b, 4, data_ov001_0231f074, data_ov001_02316840[0x33], 0);
    b->unk10C &= ~4;
    b->unk110 &= ~4;
    b->unk114 = 0;
    a->unk114 = b->unk114;
    *b->unkE4 = 0x3000;
    *a->unkE4 = *b->unkE4;
    p->unk74 = func_ov001_022f10d0;
}
