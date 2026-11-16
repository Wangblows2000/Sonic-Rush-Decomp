#include <nitro/types.h>

typedef struct UnkStruct_0230903c_o {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkStruct_0230903c_o;

typedef struct UnkStruct_0230903c {
    u8 pad0[0x780];
    UnkStruct_0230903c_o objA;
    UnkStruct_0230903c_o objB;
    u8 pad810[0xae4 - 0x810];
    s32 unkAE4;
    u8 padAE8[0xba8 - 0xae8];
    s32 unkBA8;
    s32 unkBAC;
    u8 padBB0[4];
    s32 unkBB4;
    u8 padBB8[0xbc8 - 0xbb8];
    s32 timer;
    u8 padBCC[4];
    s32 unkBD0;
} UnkStruct_0230903c;

typedef struct UnkStruct_0230903c_t {
    u8 pad0[0x148];
    s32 unk148;
} UnkStruct_0230903c_t;

extern UnkStruct_0230903c *gSpecialStageHud;

void SpecialStageHud_UpdateRingCounterThunk(s32 t);
void SpecialStageHud_SetDigitAnims(s32 t);
u16 SpecialStage_GetRingQuota(void);
UnkStruct_0230903c_t *SpecialStage_GetPlayer(void);
void AnimatorSprite_SetAnimation(UnkStruct_0230903c_o *s, u16 index);
void SpecialStageCamera_SetMode(s32 v);
void SpecialStagePlayer_Action_Celebrate(void *p);
s32 SpecialStage_GetStageIndex(void);
u16 SpecialStageHud_Unk_ov001_02314368(void);

void SpecialStageHud_UpdateCheckpoint(void) {
    UnkStruct_0230903c *g = gSpecialStageHud;
    s32 n;

    g->timer++;
    SpecialStageHud_UpdateRingCounterThunk(g->timer - 1);
    SpecialStageHud_SetDigitAnims(g->timer - 0xb4);
    switch (g->timer) {
    case 1:
        g->objB.x = 0x80;
        g->objB.y = 0x2a;
        n = SpecialStage_GetRingQuota();
        if (n <= SpecialStage_GetPlayer()->unk148) {
            AnimatorSprite_SetAnimation(&g->objB, 3);
            SpecialStageCamera_SetMode(4);
            SpecialStagePlayer_Action_Celebrate(SpecialStage_GetPlayer());
            return;
        }
        AnimatorSprite_SetAnimation(&g->objB, 10);
        g->timer = -1;
        g->unkBD0 = 0;
        return;
    case 0xf0:
        g->objA.x = 0x80;
        g->objA.y = 0x22;
        AnimatorSprite_SetAnimation(&g->objA, SpecialStage_GetStageIndex() * 2 + 2);
        g->unkBB4 = 1;
        g->unkBA8 = 1;
        g->unkBAC = 0;
        return;
    case 0x172:
        g->timer = -1;
        SpecialStageCamera_SetMode(0);
        break;
    default:
        return;
    }
    g->unkAE4 = SpecialStageHud_Unk_ov001_02314368();
}
