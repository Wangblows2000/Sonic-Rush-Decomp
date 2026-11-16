#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
    s32 unk4;
    u16 unk8;
    u16 unkA;
} UnkStruct_02369ac0;

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
    u8 padD0C[0xF4A - 0xD0C];
    s16 unkF4A;
    u8 padF4C[0xF54 - 0xF4C];
    s32 unkF54;
    s32 unkF58;
    s32 unkF5C;
    s16 unkF60;
    u8 padF62[2];
    s16 unkF64;
    u8 padF66[0x1118 - 0xF66];
    u16 unk1118;
    u8 pad111A[2];
    s32 unk111C;
    s32 unk1120;
    s32 unk1124;
} UnkStruct_Boss;

extern UnkStruct_StageObj *gPlayers;
extern UnkStruct_02369ac0 gBossStatus;
extern s32 data_ov000_02363804[];

void Boss6_UpdateMoveSpeed(UnkStruct_Boss *w);
void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void Boss6_SetCameraPickStart(UnkStruct_Boss *w);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Boss6_State_PickRaise(void);

void Boss6_State_PickStop(UnkStruct_Boss *w) {
    UnkStruct_Mdl960 *m = &w->unk960;
    s32 x;

    Boss6_UpdateMoveSpeed(w);
    w->base.unk10 += w->unkF58;
    if (w->unkF58 != 0)
        return;
    if (w->unkF4A != 0)
        return;
    w->base.unk68 = Boss6_State_PickRaise;
    Model_SetAnimation(m, 0, w->unkD08, 1, 0);
    m->unk10C &= ~4;
    m->unk114 = 0x1000;
    Boss6_SetCameraPickStart(w);
    w->unkF54 = 0;
    w->unkF58 = 0;
    w->unkF5C = 0;
    w->unkF60 = 0;
    w->unkF64 = 0;
    w->unkF4A = 0;
    w->unk1118 = 0;
    w->unk111C = w->base.unk10;
    x = gPlayers->unk10;
    if (x < 0x3000)
        x = 0x3000;
    else if (x > 0x31000)
        x = 0x31000;
    w->unk1120 = x;
    w->unk1124 = data_ov000_02363804[gBossStatus.unk8 - 1];
    Sound_PlaySeqArc(0, 0, 0x84);
}
