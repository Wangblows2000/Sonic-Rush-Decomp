#include <ov000_c/stageobj.h>

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
    u8 padF66[0x1128 - 0xF66];
    s32 unk1128;
} UnkStruct_Boss;

typedef struct {
    u8 pad[8];
    u16 unk8;
} UnkStruct_02369ac0;

extern UnkStruct_Boss *gBoss6;
extern UnkStruct_02369ac0 gBossStatus;
extern s32 data_ov000_02363824[];

void NNS_G3dRenderObjSetCallBack(void *p, void (*fn)(void), s32 a, s32 b, s32 c);
void func_ov000_022e5df0(void);
void Boss6_State_Unk_ov000_022e3674(void);

void func_ov000_022e3718(void) {
    UnkStruct_Boss *w = gBoss6;

    NNS_G3dRenderObjSetCallBack(w->unk9F0, func_ov000_022e5df0, 0, 6, 3);
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
    w->base.unk68 = Boss6_State_Unk_ov000_022e3674;
    w->unk1128 = data_ov000_02363824[gBossStatus.unk8 - 1];
}
