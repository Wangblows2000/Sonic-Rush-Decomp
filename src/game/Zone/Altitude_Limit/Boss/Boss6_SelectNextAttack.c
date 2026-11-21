#include <game/Stage/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0xD4C - 0x20C];
    s16 unkD4C;
    s16 unkD4E;
    u8 padD50[0xF44 - 0xD50];
    u16 unkF44;
    u16 unkF46;
    u16 unkF48;
} UnkStruct_022e1b48;

typedef struct {
    u32 unk0;
    s32 unk4;
    u16 unk8;
    u16 unkA;
} UnkStruct_02369ac0;

extern UnkStruct_022e1b48 *gBoss6;
extern UnkStruct_02369ac0 gBossStatus;
extern u16 data_ov000_02363918[][8];
extern u16 data_ov000_02363998[][8];
extern u16 data_ov000_023639d8[][8];

void Boss6_Action_StartPartD4Attack(void);
void Boss6_Action_StartPickAttack(void);
void Boss6_Action_StartTilt(void);
void Boss6_Action_StartEdgeJump(void);
void Boss6_Action_StartMove(void);

static inline void Pick(UnkStruct_022e1b48 *b, u16 (*tbl)[8]) {
    b->unkF46 = tbl[b->unkD4E][b->unkD4C];
    b->unkD4C++;
    if (b->unkD4C >= 8) {
        b->unkD4E = Rand() & 3;
        b->unkD4C = 0;
    }
}

void Boss6_SelectNextAttack(s32 mode) {
    UnkStruct_022e1b48 *b = gBoss6;
    s32 hp = gBossStatus.unk8;

    b->unkF48 = mode;
    switch (mode) {
    case 0:
        switch (b->unkF46) {
        case 1:
            gBossStatus.unk4 = 0x1e;
            Boss6_Action_StartPartD4Attack();
            return;
        case 2:
            gBossStatus.unk4 = 0x1f;
            Boss6_Action_StartPickAttack();
            return;
        case 3:
            gBossStatus.unk4 = 0x20;
            Boss6_Action_StartTilt();
            return;
        case 4:
            gBossStatus.unk4 = 0x21;
            Boss6_Action_StartEdgeJump();
            return;
        }
        break;
    case 1:
        b->unkF44++;
        if (hp >= 6)
            Pick(b, data_ov000_02363918);
        else if (hp >= 3)
            Pick(b, data_ov000_02363998);
        else
            Pick(b, data_ov000_023639d8);
        gBossStatus.unk4 = 0x1c;
        Boss6_Action_StartMove();
        break;
    default:
        if (Rand() & 1) {
            b->unkF46 = 1;
        } else if (hp >= 6) {
            Pick(b, data_ov000_02363918);
        } else if (hp >= 3) {
            Pick(b, data_ov000_02363998);
        } else {
            Pick(b, data_ov000_023639d8);
        }
        gBossStatus.unk4 = 0x1c;
        Boss6_Action_StartMove();
        break;
    }
}
