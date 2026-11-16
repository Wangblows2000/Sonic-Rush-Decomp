#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x440 - 0x20C];
    void *unk440;
    u8 pad444[0x480 - 0x444];
    void *unk480;
    u8 pad484[4];
    UnkStruct_StageObj *unk488;
} UnkStruct_022e72ec;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
} UnkStruct_Boss;

typedef struct {
    u8 pad[4];
    s32 unk4;
} UnkStruct_02369ac0;

extern UnkStruct_Boss *gBoss7;
extern UnkStruct_02369ac0 gBossStatus;

void Boss7_Unk_ov000_022ee198(UnkStruct_022e72ec *s, s32 st);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void EffTrail_Unk_ov000_02326f20(void *p);
void Boss7PartD5_MoveState_Unk_ov000_022e72cc(void);

void Boss7PartD5_MoveState_Unk_ov000_022e72ec(UnkStruct_022e72ec *w) {
    UnkStruct_Boss *b = gBoss7;

    Boss7_Unk_ov000_022ee198(w, 0xf);
    Sound_PlaySeqArc(0, 0, 0x7b);
    w->base.unk1F0 = 0xb4;
    w->base.unk58 |= 0x90;
    w->base.unk30 = -0x300;
    if (w->base.unk5C & 1)
        w->base.unk2E = 0x180;
    else
        w->base.unk2E = -0x180;
    if (w->unk480 != NULL) {
        EffTrail_Unk_ov000_02326f20(w->unk480);
        w->unk480 = NULL;
    }
    if (w->unk488 != NULL) {
        w->unk488->unk54 |= 8;
        w->unk488 = NULL;
    }
    b->unk20C &= ~0x10;
    gBossStatus.unk4 = 0x2d;
    w->unk440 = Boss7PartD5_MoveState_Unk_ov000_022e72cc;
}
