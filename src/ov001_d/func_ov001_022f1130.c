#include <nitro/types.h>

typedef struct UnkStruct_022f1130_o {
    u8 pad0[0xe4];
    s32 *unkE4;
    u8 padE8[0x10c - 0xe8];
    u8 unk10C;
    u8 pad10D[3];
    u8 unk110;
    u8 pad111[3];
    s32 unk114;
    s32 unk118;
} UnkStruct_022f1130_o;

typedef struct UnkStruct_022f1130_q {
    u8 pad0[0x74];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f1130_o *unk84;
} UnkStruct_022f1130_q;

typedef struct UnkStruct_022f1130_s {
    u8 pad0[0x18];
    UnkStruct_022f1130_q *unk18;
} UnkStruct_022f1130_s;

typedef struct UnkStruct_022f1130 {
    u8 pad0[0x74];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f1130_o *unk84;
    u8 pad88[0x160 - 0x88];
    u32 unk160;
    u8 pad164[0x1cc - 0x164];
    s32 unk1CC;
    u8 pad1D0[0x1dc - 0x1d0];
    UnkStruct_022f1130_s *unk1DC;
} UnkStruct_022f1130;

typedef struct UnkStruct_022f1130_t {
    u8 pad0[0x60];
    u16 unk60;
    u16 unk62;
    u16 unk64;
    u16 unk66;
} UnkStruct_022f1130_t;

extern void *data_ov001_0231f068;
extern void *data_ov001_0231f074;
extern UnkStruct_022f1130_t data_ov001_02316840;

void func_ov001_022f10d0(void);
void func_02031a68(void *p, s32 a, void *b, s32 c, s32 d);

void func_ov001_022f1130(UnkStruct_022f1130 *p) {
    s32 d = p->unk1CC;
    UnkStruct_022f1130_o *a = p->unk84;
    UnkStruct_022f1130_o *b = p->unk1DC->unk18->unk84;

    if ((d < 0 && *a->unkE4 + d <= 0xb4000) || (d > 0 && *a->unkE4 + d >= 0xb4000)) {
        func_02031a68(a, 0, data_ov001_0231f068, data_ov001_02316840.unk60, 0);
        func_02031a68(a, 4, data_ov001_0231f074, data_ov001_02316840.unk62, 0);
        a->unk10C &= ~4;
        a->unk110 &= ~4;
        func_02031a68(b, 0, data_ov001_0231f068, data_ov001_02316840.unk64, 0);
        func_02031a68(b, 4, data_ov001_0231f074, data_ov001_02316840.unk66, 0);
        b->unk10C &= ~4;
        b->unk110 &= ~4;
        b->unk114 = 0;
        a->unk114 = b->unk114;
        *b->unkE4 = 0x33000;
        *a->unkE4 = *b->unkE4;
        p->unk74 = func_ov001_022f10d0;
        return;
    }
    *a->unkE4 += d;
    *b->unkE4 += p->unk1CC;
}
