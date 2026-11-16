#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x114];
    s32 unk114;
    u8 pad118[0x16C - 0x118];
} UnkStruct_Mdl;

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Mdl mdl;
    s32 unk378;
    u16 unk37C;
    u8 pad37E[2];
    UnkStruct_StageObj *unk380;
    s32 unk384;
    void *unk388;
    void *unk38C;
    u8 pad390[0x398 - 0x390];
    void *unk398;
} UnkStruct_022ec364;

extern s16 data_ov000_02363b64[];

void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void *NNS_G3dGetTex(void *p);
UnkStruct_StageObj *BossEffect_CreateSuction(s32 a, s32 b, s32 c, s32 d);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Boss7PartD6_State_WaitAnimEnd_ov000_022ec34c(void);

void Boss7PartD6_State_Unk_ov000_022ec364(UnkStruct_022ec364 *w) {
    UnkStruct_Mdl *m = &w->mdl;

    Model_SetAnimation(m, 1, w->unk38C, 0, 0);
    Model_SetAnimation(m, 2, w->unk398, 0, (s32)NNS_G3dGetTex(w->unk388));
    w->mdl.unk114 = 0x1000;
    if (w->unk378 == 0) {
        w->unk380 = BossEffect_CreateSuction(1, w->base.unk10, w->base.unk14, w->base.unk18);
        w->unk380->unk5C |= 4;
        w->unk380->unkC = -1;
        Sound_PlaySeqArc(w->unk384, 0, 0x98);
    } else {
        w->mdl.unk114 = data_ov000_02363b64[w->unk37C];
        Sound_PlaySeqArc(w->unk384, 0, 0x99);
    }
    w->base.unk68 = Boss7PartD6_State_WaitAnimEnd_ov000_022ec34c;
}
