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
    u8 pad390[0x394 - 0x390];
    void *unk394;
    u8 pad398[0x3A0 - 0x398];
    void *unk3A0;
} UnkStruct_022ec1d0;

extern s16 data_ov000_02363b64[];

void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void *NNS_G3dGetTex(void *p);
void NNS_SndPlayerStopSeq(s32 a, s32 b);
void Boss7PartD6_State_Unk_ov000_022ec1b8(void);

void func_ov000_022ec1d0(UnkStruct_022ec1d0 *w) {
    UnkStruct_Mdl *m = &w->mdl;

    Model_SetAnimation(m, 1, w->unk394, 0, 0);
    Model_SetAnimation(m, 2, w->unk3A0, 0, (s32)NNS_G3dGetTex(w->unk388));
    w->base.coll1.unkA &= ~4;
    w->base.unk5C &= ~4;
    if (w->unk378 == 0) {
        w->unk380->unk54 |= 8;
        NNS_SndPlayerStopSeq(w->unk384, 0x3c);
    } else {
        w->mdl.unk114 = data_ov000_02363b64[w->unk37C];
    }
    w->base.unk68 = Boss7PartD6_State_Unk_ov000_022ec1b8;
}
