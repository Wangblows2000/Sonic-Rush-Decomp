#include <game/Stage/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x9F0 - 0x20C];
    u8 unk9F0[0xF54 - 0x9F0];
    s32 unkF54;
    s32 unkF58;
    s32 unkF5C;
    s16 unkF60;
    u8 padF62[2];
    s16 unkF64;
} UnkStruct_Boss;

extern UnkStruct_StageObj *gPlayers;
extern UnkStruct_Boss *gBoss6;

void NNS_G3dRenderObjSetCallBack(void *p, void (*fn)(void), s32 a, s32 b, s32 c);
void Boss6_NodeCallback_Store4Nodes_ov000_022e60a4(void);
void Boss6_State_PickStop(void);
void Boss6_State_PickApproach(void);

void Boss6_Action_StartPickAttack(void) {
    UnkStruct_Boss *w = gBoss6;
    s32 x = gPlayers->unk10;

    NNS_G3dRenderObjSetCallBack(w->unk9F0, Boss6_NodeCallback_Store4Nodes_ov000_022e60a4, 0, 6, 3);
    if (x < w->base.unk10 + 0x3200 && x > w->base.unk10 - 0x3200) {
        w->base.unk68 = Boss6_State_PickStop;
        if (w->unkF54 >= 0) {
            if (w->unkF58 > 0) {
                w->unkF54 = -w->unkF54;
                w->unkF60 = -w->unkF60;
            }
        } else {
            if (w->unkF58 < 0) {
                w->unkF54 = -w->unkF54;
                w->unkF60 = -w->unkF60;
            }
        }
        w->unkF64 = 0;
        w->unkF5C = 0;
        return;
    }
    w->base.unk68 = Boss6_State_PickApproach;
}
