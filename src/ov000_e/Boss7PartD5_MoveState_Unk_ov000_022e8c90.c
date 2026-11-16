#include <ov000_c/stageobj.h>

typedef struct UnkStruct_022e8c90 {
    UnkStruct_StageObj base;
    u8 pad20C[0x3D0 - 0x20C];
    s32 unk3D0;
    u8 pad3D4[0x440 - 0x3D4];
    void (*unk440)(struct UnkStruct_022e8c90 *);
} UnkStruct_022e8c90;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
} UnkStruct_Boss;

typedef struct {
    u8 pad[0x114];
    s32 unk114;
} UnkStruct_124;

typedef struct {
    u8 pad[4];
    s32 unk4;
} UnkStruct_02369ac0;

extern UnkStruct_Boss *gBoss7;
extern UnkStruct_StageObj *gPlayers;
extern UnkStruct_02369ac0 gBossStatus;

void Boss7_Unk_ov000_022ee198(UnkStruct_022e8c90 *s, s32 st);
void Boss7_Unk_ov000_022e9b70(UnkStruct_022e8c90 *s, s32 a, s32 b);
void Boss7PartD5_MoveState_Unk_ov000_022e98d8(UnkStruct_022e8c90 *s);
void Boss7PartD5_MoveState_Unk_ov000_022e8bac(UnkStruct_022e8c90 *s);

void Boss7PartD5_MoveState_Unk_ov000_022e8c90(UnkStruct_022e8c90 *w) {
    UnkStruct_Boss *b = gBoss7;

    if (w->base.unk58 & 1) {
        w->unk440 = Boss7PartD5_MoveState_Unk_ov000_022e98d8;
        return;
    }
    w->base.unk2E = 0;
    w->base.unk30 = -0x20;
    w->base.unk58 &= ~0x80;
    Boss7_Unk_ov000_022ee198(w, 0x32);
    Boss7_Unk_ov000_022e9b70(w, 0, 0);
    w->base.unk5C |= 4;
    w->base.spr0.unk2A = 0;
    w->base.spr1.unk2A = 0;
    ((UnkStruct_124 *)w->base.unk124)->unk114 = 0;
    if (w->unk3D0 == 0)
        b->unk20C |= 0x10;
    if (w->base.unk10 > gPlayers->unk10)
        w->base.unk5C |= 1;
    else
        w->base.unk5C &= ~1;
    gBossStatus.unk4 = 0x2b;
    w->unk440 = Boss7PartD5_MoveState_Unk_ov000_022e8bac;
    w->unk440(w);
}
