#include <nitro/types.h>

typedef struct UnkStruct_02308e90_o {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkStruct_02308e90_o;

typedef struct UnkStruct_02308e90 {
    u8 pad0[0x780];
    UnkStruct_02308e90_o objA;
    UnkStruct_02308e90_o objB;
    u8 pad810[0xbcc - 0x810];
    s32 timer;
    s32 unkBD0;
} UnkStruct_02308e90;

typedef struct UnkStruct_02308e90_t {
    u8 pad0[0x148];
    s32 unk148;
} UnkStruct_02308e90_t;

extern UnkStruct_02308e90 *gSpecialStageHud;
extern s16 gDisplaySub[];
extern u8 gBGMSoundHandle[];

void func_ov001_023085f4(s32 t);
void SpecialStageHud_UpdateQuotaCounter(void);
s32 func_ov001_02314368(void);
UnkStruct_02308e90_t *SpecialStage_GetPlayer(void);
void AnimatorSprite_SetAnimation(UnkStruct_02308e90_o *s, u16 index);
void SpecialStagePlayer_Action_Celebrate(void *p);
void NNS_SndPlayerStopSeq(void *p, s32 v);
void func_ov001_0230808c(void);

void SpecialStageHud_UpdateGoal(void) {
    UnkStruct_02308e90 *g = gSpecialStageHud;
    s16 b;

    g->timer++;
    if (g->timer >= 500) {
        b = g->timer - 500;
        if (b > 0x10)
            b = 0x10;
        gDisplaySub[0x24] = b;
    }
    func_ov001_023085f4(g->timer - 1);
    SpecialStageHud_UpdateQuotaCounter();
    switch (g->timer) {
    case 1:
        g->objB.x = 0x80;
        g->objB.y = 0x2a;
        if (func_ov001_02314368() <= SpecialStage_GetPlayer()->unk148) {
            AnimatorSprite_SetAnimation(&g->objB, 3);
            SpecialStagePlayer_Action_Celebrate(SpecialStage_GetPlayer());
            return;
        }
        AnimatorSprite_SetAnimation(&g->objB, 10);
        g->timer = -1;
        g->unkBD0 = 0;
        return;
    case 0x1cc:
        NNS_SndPlayerStopSeq(gBGMSoundHandle, 0x3c);
        return;
    case 0x208:
        g->timer = -1;
        func_ov001_0230808c();
        return;
    }
}
