#include <nitro/types.h>

typedef struct UnkStruct_022c9778_3 {
    u8 pad[0x216];
    u16 unk216;
    u8 pad0[0x242 - 0x218];
    u16 unk242;
    u8 pad1[0x29a - 0x244];
    u16 unk29A;
    u8 pad2[0x304 - 0x29c];
    s32 unk304;
    u8 pad3[4];
    u8 unk30C[4];
} UnkStruct_022c9778_3;

typedef struct UnkStruct_022c9778_2 {
    u8 pad[0x214];
    UnkStruct_022c9778_3 *unk214;
} UnkStruct_022c9778_2;

typedef struct UnkStruct_022c9778_4 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c9778_4;

typedef struct UnkStruct_022c9778_5 {
    u8 pad[8];
    u16 unk8;
} UnkStruct_022c9778_5;

typedef struct UnkStruct_022c9778 UnkStruct_022c9778;
struct UnkStruct_022c9778 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022c9778 *s);
    u8 pad1[0x20c - 0x6c];
    u16 unk20C;
    u8 pad1b[0x218 - 0x20e];
    u16 unk218;
    u8 pad2[0x220 - 0x21a];
    s32 unk220;
    u8 pad3[4];
    u8 unk228[4];
};

extern UnkStruct_022c9778_2 *gBoss1;
extern UnkStruct_022c9778_4 gGameState;
extern UnkStruct_022c9778_5 gBossStatus;
extern s16 data_ov000_02362d04[][8];
extern s16 data_ov000_02362ce4[][8];

BOOL Boss1_Unk_ov000_022cc6d4(void);
void Boss1_SetPartAnimation(void *p, s32 a, s32 b, s32 c, s32 d);
void Boss1Neck_State_SlamImpactShake(UnkStruct_022c9778 *s);

void Boss1Neck_State_SlamImpact(UnkStruct_022c9778 *s) {
    UnkStruct_022c9778_3 *p = gBoss1->unk214;
    s32 v;
    BOOL b;
    s32 i;
    if (Boss1_Unk_ov000_022cc6d4()) {
        b = TRUE;
        if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
            b = FALSE;
        if (b)
            i = 1;
        else
            i = gGameState.unk14;
        v = data_ov000_02362d04[i][gBossStatus.unk8 - 1];
    } else {
        b = TRUE;
        if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
            b = FALSE;
        if (b)
            i = 1;
        else
            i = gGameState.unk14;
        v = data_ov000_02362ce4[i][gBossStatus.unk8 - 1];
    }
    Boss1_SetPartAnimation(s->unk228, s->unk220, 0x11, v, 0);
    Boss1_SetPartAnimation(p->unk30C, p->unk304, 4, v, 0);
    p->unk216 |= 4;
    p->unk29A |= 4;
    p->unk242 &= ~4;
    s->unk20C = 0x40;
    s->unk218 = 0;
    s->func = Boss1Neck_State_SlamImpactShake;
}
