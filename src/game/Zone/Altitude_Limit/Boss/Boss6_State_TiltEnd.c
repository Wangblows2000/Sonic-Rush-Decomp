#include <game/Stage/stageobj.h>

typedef struct {
    u8 pad0[0x90];
    u8 unk90[0x10C - 0x90];
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
    u8 padD0C[0xF68 - 0xD0C];
    u16 unkF68;
    u16 unkF6A;
    u16 unkF6C;
    u8 padF6E[0x1118 - 0xF6E];
    s32 unk1118;
} UnkStruct_Boss;

void NNS_G3dRenderObjSetCallBack(void *p, void (*fn)(void), s32 a, s32 b, s32 c);
void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void Boss6_NodeCallback_Store2Nodes_ov000_022e62fc(void);
void Boss6_State_WaitSelectAttack3(void);

void Boss6_State_TiltEnd(UnkStruct_Boss *w) {
    UnkStruct_Mdl960 *m = &w->unk960;

    if (!(m->unk10C & 2))
        return;
    NNS_G3dRenderObjSetCallBack(m->unk90, Boss6_NodeCallback_Store2Nodes_ov000_022e62fc, 0, 6, 3);
    w->base.unk68 = Boss6_State_WaitSelectAttack3;
    Model_SetAnimation(m, 0, w->unkD08, 0xc, 0);
    m->unk114 = 0x1000;
    w->unkF68 = 0x549f;
    w->unkF6A = 0;
    w->unkF6C = 0x549f;
    w->base.unk10 = w->unk1118;
}
