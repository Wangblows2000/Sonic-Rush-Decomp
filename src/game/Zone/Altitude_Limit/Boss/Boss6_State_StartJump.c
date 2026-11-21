#include <game/Stage/stageobj.h>

typedef struct {
    u8 pad0[0x10C];
    u8 unk10C;
    u8 pad10D[0x114 - 0x10D];
    s32 unk114;
} UnkStruct_Mdl960;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x960 - 0x20C];
    UnkStruct_Mdl960 unk960;
    u8 pad[0xD08 - 0x960 - sizeof(UnkStruct_Mdl960)];
    void *unkD08;
    u8 padD0C[0xD5E - 0xD0C];
    u16 unkD5E;
    u8 padD60[0x111C - 0xD60];
    s16 unk111C;
    u8 pad111E[2];
    s32 unk1120;
    s16 unk1124;
} UnkStruct_Boss;

typedef struct {
    u8 pad[8];
    u16 unk8;
} UnkStruct_02369ac0;

extern UnkStruct_02369ac0 gBossStatus;
extern s32 data_ov000_02363844[];
extern UnkStruct_StageObj *gPlayers;

void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void Boss6_SetCameraJumpStart(UnkStruct_Boss *w);
void Boss6_State_JumpRise(void);

void Boss6_State_StartJump(UnkStruct_Boss *w) {
    UnkStruct_Mdl960 *m = &w->unk960;

    if (!(m->unk10C & 2))
        return;
    w->base.unk68 = Boss6_State_JumpRise;
    Model_SetAnimation(m, 0, w->unkD08, 0x11, 0);
    m->unk10C &= ~4;
    m->unk114 = 0x1000;
    w->unk111C = -0x1e00;
    w->unk1120 = data_ov000_02363844[gBossStatus.unk8 - 1];
    if (gPlayers->unk10 - w->base.unk10 < 0) {
        if (w->unkD5E != 0)
            w->unk1124 = 0x38e;
        else
            w->unk1124 = -0x38e;
    } else {
        if (w->unkD5E != 0)
            w->unk1124 = -0x38e;
        else
            w->unk1124 = 0x38e;
    }
    Boss6_SetCameraJumpStart(w);
}
