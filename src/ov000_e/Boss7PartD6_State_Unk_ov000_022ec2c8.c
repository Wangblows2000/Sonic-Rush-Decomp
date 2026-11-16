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
    void *unk390;
    u8 pad394[0x39C - 0x394];
    void *unk39C;
} UnkStruct_022ec2c8;

extern s16 data_ov000_02363b64[];

void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void *NNS_G3dGetTex(void *p);
void NNS_SndPlayerStopSeq(s32 a, s32 b);
void Boss7PartD6_State_Unk_ov000_022ec294(void);

void Boss7PartD6_State_Unk_ov000_022ec2c8(UnkStruct_022ec2c8 *w) {
    UnkStruct_Mdl *m = &w->mdl;

    Model_SetAnimation(m, 1, w->unk390, 0, 0);
    Model_SetAnimation(m, 2, w->unk39C, 0, (s32)NNS_G3dGetTex(w->unk388));
    w->mdl.unk114 = 0x1000;
    w->base.coll1.unkA |= 4;
    w->base.unk5C |= 4;
    w->base.unk68 = Boss7PartD6_State_Unk_ov000_022ec294;
}
