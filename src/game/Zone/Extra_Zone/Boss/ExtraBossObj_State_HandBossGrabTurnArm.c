#include <nitro/types.h>

typedef struct UnkStruct_022f0cbc_o {
    u8 pad0[0xe4];
    s32 *unkE4;
    u8 padE8[0x10c - 0xe8];
    u8 unk10C;
    u8 pad10D[3];
    u8 unk110;
    u8 pad111[3];
    s32 unk114;
    s32 unk118;
} UnkStruct_022f0cbc_o;

typedef struct UnkStruct_022f0cbc_q {
    u8 pad0[0x74];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f0cbc_o *unk84;
} UnkStruct_022f0cbc_q;

typedef struct UnkStruct_022f0cbc_s {
    u8 pad0[0x18];
    UnkStruct_022f0cbc_q *unk18;
} UnkStruct_022f0cbc_s;

typedef struct UnkStruct_022f0cbc {
    u8 pad0[0x74];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f0cbc_o *unk84;
    u8 pad88[0x160 - 0x88];
    u32 unk160;
    u8 pad164[0x1cc - 0x164];
    s32 unk1CC;
    u8 pad1D0[0x1dc - 0x1d0];
    UnkStruct_022f0cbc_s *unk1DC;
} UnkStruct_022f0cbc;

typedef struct UnkStruct_022f0cbc_t {
    u8 pad0[0x78];
    u16 unk78;
    u16 unk7A;
    u16 unk7C;
    u16 unk7E;
} UnkStruct_022f0cbc_t;

extern void *data_ov001_0231f068;
extern void *data_ov001_0231f074;
extern UnkStruct_022f0cbc_t gExtraBossHandBossAnimTable;

void ExtraBossObj_State_HandBossGrabHold(void);
void ExtraBossObj_State_None(void);
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

void ExtraBossObj_State_HandBossGrabTurnArm(UnkStruct_022f0cbc *p) {
    UnkStruct_022f0cbc_q *q = p->unk1DC->unk18;
    s32 d = p->unk1CC;
    UnkStruct_022f0cbc_o *a = p->unk84;
    UnkStruct_022f0cbc_o *b = q->unk84;

    if ((d < 0 && *a->unkE4 + d <= 0xb4000) || (d > 0 && *a->unkE4 + d >= 0xb4000)) {
        Model_SetAnimation(a, 0, data_ov001_0231f068, gExtraBossHandBossAnimTable.unk78, 0);
        Model_SetAnimation(a, 4, data_ov001_0231f074, gExtraBossHandBossAnimTable.unk7A, 0);
        a->unk10C &= ~4;
        a->unk110 &= ~4;
        Model_SetAnimation(b, 0, data_ov001_0231f068, gExtraBossHandBossAnimTable.unk7C, 0);
        Model_SetAnimation(b, 4, data_ov001_0231f074, gExtraBossHandBossAnimTable.unk7E, 0);
        b->unk10C &= ~4;
        b->unk110 &= ~4;
        b->unk118 = 0x1000;
        b->unk114 = 0x1000;
        a->unk114 = b->unk114;
        p->unk74 = ExtraBossObj_State_HandBossGrabHold;
        q->unk74 = ExtraBossObj_State_None;
        p->unk160 &= ~0x200;
        return;
    }
    *a->unkE4 += d;
    *b->unkE4 += p->unk1CC;
}
