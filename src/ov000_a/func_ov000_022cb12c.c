#include <nitro/types.h>

typedef struct UnkStruct_022cb12c_3 {
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
} UnkStruct_022cb12c_3;

typedef struct UnkStruct_022cb12c_4 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x216 - 0x58];
    u16 unk216;
    u8 pad2[0x242 - 0x218];
    u16 unk242;
    u8 pad3[0x26e - 0x244];
    u16 unk26E;
    u8 pad3b[0x29a - 0x270];
    u16 unk29A;
    u8 pad4[0x304 - 0x29c];
    s32 unk304;
    u8 pad4b[4];
    u8 unk30C[0x420 - 0x30c];
    s32 unk420;
} UnkStruct_022cb12c_4;

typedef struct UnkStruct_022cb12c UnkStruct_022cb12c;
struct UnkStruct_022cb12c {
    u8 pad[0x54];
    u32 unk54;
    u8 pad0[0x68 - 0x58];
    void (*func)(UnkStruct_022cb12c *s);
    u8 pad1[0x210 - 0x6c];
    UnkStruct_022cb12c_3 *unk210;
    UnkStruct_022cb12c_4 *unk214;
    u8 pad2[0x220 - 0x218];
    u16 unk220;
    u16 unk222;
    u8 pad3[0x350 - 0x224];
    s32 unk350;
};

typedef struct UnkStruct_022cb12c_pl {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad1[0x3b2 - 0x18];
    u16 unk3B2;
} UnkStruct_022cb12c_pl;

typedef struct UnkStruct_022cb12c_5 {
    s32 unk0;
    u8 pad[0x18 - 4];
    u32 unk18;
} UnkStruct_022cb12c_5;

typedef struct UnkStruct_022cb12c_6 {
    u8 pad[4];
    s32 unk4;
} UnkStruct_022cb12c_6;

extern UnkStruct_022cb12c_6 gBossStatus;
extern u32 gRandSeed;

void func_ov000_022cc00c(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_022cc224(void);
void Boss1_State_Unk_ov000_022cb0f4(UnkStruct_022cb12c *s);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void func_ov000_022cb12c(UnkStruct_022cb12c *s) {
    UnkStruct_022cb12c_4 *p = s->unk214;
    func_ov000_022cc00c(s->unk210->unk228, s->unk210->unk220, 0xd, 0x1000, 1);
    func_ov000_022cc00c(p->unk30C, p->unk304, 0, 0x1000, 1);
    s->unk220 = (Rand() & 0x3f) + 0x1e;
    func_ov000_022cc224();
    p->unk216 &= ~4;
    p->unk242 &= ~4;
    p->unk26E &= ~4;
    p->unk29A &= ~4;
    gBossStatus.unk4 = 1;
    s->func = Boss1_State_Unk_ov000_022cb0f4;
}
