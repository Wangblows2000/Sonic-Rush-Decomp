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
    u32 unk20C;
    u8 pad210[0x960 - 0x210];
    UnkStruct_Mdl960 unk960;
    u8 pad[0xD08 - 0x960 - sizeof(UnkStruct_Mdl960)];
    void *unkD08;
    u8 padD0C[0xE72 - 0xD0C];
    u16 unkE72;
    u8 padE74[0xECA - 0xE74];
    u16 unkECA;
    u8 padECC[0xF50 - 0xECC];
    s32 unkF50;
} UnkStruct_Boss;

void NNS_G3dRenderObjSetCallBack(void *p, void (*fn)(void), s32 a, s32 b, s32 c);
void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void Boss6_NodeCallback_PelvisAndPro(void);
void Boss6_State_TiltLand(void);

void Boss6_State_TiltJump(UnkStruct_Boss *w) {
    UnkStruct_Mdl960 *m = &w->unk960;

    w->unkF50--;
    if (w->unkF50 > 0)
        return;
    NNS_G3dRenderObjSetCallBack(m->unk90, Boss6_NodeCallback_PelvisAndPro, 0, 6, 3);
    w->base.unk68 = Boss6_State_TiltLand;
    Model_SetAnimation(m, 0, w->unkD08, 8, 0);
    m->unk114 = 0x1000;
    w->unkE72 |= 4;
    w->unkE72 &= ~0x1000;
    w->unkECA |= 4;
    w->unkECA &= ~0x1000;
    w->unk20C |= 8;
}
