#include <nitro/types.h>

typedef struct UnkStruct_022cae4c_3 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x54 - 0x14];
    u32 unk54;
    u8 pad1[0x220 - 0x58];
    s32 unk220;
    u8 pad1b[4];
    u8 unk228[0x334 - 0x228];
    u8 unk334;
    u8 pad1c[0x33c - 0x335];
    s32 unk33C;
} UnkStruct_022cae4c_3;

typedef struct UnkStruct_022cae4c_4 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x216 - 0x58];
    u16 unk216;
    u8 pad2[0x242 - 0x218];
    u16 unk242;
    u8 pad3[0x29a - 0x244];
    u16 unk29A;
    u8 pad4[0x304 - 0x29c];
    s32 unk304;
    u8 pad4b[4];
    u8 unk30C[0x420 - 0x30c];
    s32 unk420;
} UnkStruct_022cae4c_4;

typedef struct UnkStruct_022cae4c UnkStruct_022cae4c;
struct UnkStruct_022cae4c {
    u8 pad[0x54];
    u32 unk54;
    u8 pad0[0x68 - 0x58];
    void (*func)(UnkStruct_022cae4c *s);
    u8 pad1[0x210 - 0x6c];
    UnkStruct_022cae4c_3 *unk210;
    UnkStruct_022cae4c_4 *unk214;
    u8 pad2[0x220 - 0x218];
    u16 unk220;
    u16 unk222;
    u8 pad3[0x350 - 0x224];
    s32 unk350;
};

typedef struct UnkStruct_022cae4c_pl {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad1[0x3b2 - 0x18];
    u16 unk3B2;
} UnkStruct_022cae4c_pl;

typedef struct UnkStruct_022cae4c_5 {
    s32 unk0;
    u8 pad[0x18 - 4];
    u32 unk18;
} UnkStruct_022cae4c_5;

typedef struct UnkStruct_022cae4c_6 {
    u8 pad[4];
    s32 unk4;
} UnkStruct_022cae4c_6;

extern UnkStruct_022cae4c_6 data_ov000_02369ac0;
extern u32 data_0208fe70;

void func_ov000_022cc00c(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_022cc224(void);
void func_020446b0(s32 a);
void func_ov000_022cc110(s32 a);
void func_02036754(s32 a, s32 b, s32 c);
void func_020366f4(s32 a, s32 b, s32 c);
void func_ov000_022cad94(UnkStruct_022cae4c *s);

static inline u16 Rand(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}

void func_ov000_022cae4c(UnkStruct_022cae4c *s) {
    data_ov000_02369ac0.unk4 = 5;
    func_ov000_022cc00c(s->unk210->unk228, s->unk210->unk220, 0x1e, 0x1000, 0);
    func_ov000_022cc00c(s->unk214->unk30C, s->unk214->unk304, 0xb, 0x1000, 0);
    s->unk214->unk216 &= ~4;
    s->unk214->unk29A &= ~4;
    s->unk214->unk242 &= ~4;
    func_ov000_022cc224();
    func_020446b0(0);
    func_ov000_022cc110(0x1000);
    func_02036754(0, 0, 0x7b);
    func_020366f4(0, 0, (Rand() & 1) ? 0xce : 0xcf);
    s->unk220 = 0;
    s->unk222 = 0;
    s->func = func_ov000_022cad94;
}
