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

extern UnkStruct_022cae4c_6 gBossStatus;
extern u32 gRandSeed;

void Boss1_SetPartAnimation(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_022cc224(void);
void ScreenShake_Start(s32 a);
void func_ov000_022cc110(s32 a);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Sound_PlayVoice(s32 a, s32 b, s32 c);
void Boss1_State_Hurt(UnkStruct_022cae4c *s);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void Boss1_Action_Hurt(UnkStruct_022cae4c *s) {
    gBossStatus.unk4 = 5;
    Boss1_SetPartAnimation(s->unk210->unk228, s->unk210->unk220, 0x1e, 0x1000, 0);
    Boss1_SetPartAnimation(s->unk214->unk30C, s->unk214->unk304, 0xb, 0x1000, 0);
    s->unk214->unk216 &= ~4;
    s->unk214->unk29A &= ~4;
    s->unk214->unk242 &= ~4;
    func_ov000_022cc224();
    ScreenShake_Start(0);
    func_ov000_022cc110(0x1000);
    Sound_PlaySeqArc(0, 0, 0x7b);
    Sound_PlayVoice(0, 0, (Rand() & 1) ? 0xce : 0xcf);
    s->unk220 = 0;
    s->unk222 = 0;
    s->func = Boss1_State_Hurt;
}
