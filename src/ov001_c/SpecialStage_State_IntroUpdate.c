#include <nitro/types.h>

typedef struct UnkStruct_0230b658_o {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkStruct_0230b658_o;

typedef struct UnkStruct_0230b658 {
    u8 pad0[0x18];
    void *unk18;
    u8 pad1C[0x2c - 0x1c];
    s32 timer;
    s32 unk30;
    s32 unk34;
    u8 pad38[0xf8 - 0x38];
    UnkStruct_0230b658_o objA;
    UnkStruct_0230b658_o objB;
} UnkStruct_0230b658;

typedef struct UnkStruct_0230b658_k {
    u8 pad0[0x10];
    u32 unk10;
} UnkStruct_0230b658_k;

extern UnkStruct_0230b658 *gSpecialStageWork;
extern s16 gDisplayMain[];
extern s16 gDisplaySub[];
extern UnkStruct_0230b658_k gTouchState;
extern void *gBGMSoundHandle;

void AnimatorSprite_ProcessAnimation(void *c, u32 a, u32 b);
void SpecialStagePlayer_Update(void *w);
void SpecialStageCamera_Update(void);
void Sound_PlaySeqArc(u32 *p, u32 a, u32 b);
void SpecialStageCamera_SetMode(s32 v);
void NNS_SndPlayerStopSeq(void **pp, s32 a);
void SpecialStage_StartPlay(void);

void SpecialStage_State_IntroUpdate(void) {
    UnkStruct_0230b658 *g = gSpecialStageWork;
    UnkStruct_0230b658_o *a = &g->objA;
    UnkStruct_0230b658_o *b = &g->objB;
    s16 v;

    if (g->timer <= 0x10) {
        gDisplayMain[0x24] = 0x10 - g->timer;
        gDisplaySub[0x24] = 0x10 - g->timer;
    }
    if (g->timer > 0x5c) {
        v = g->timer - 0x5c;
        if (v > 0x10)
            v = 0x10;
        gDisplaySub[0x24] = v;
    }
    if (g->timer <= 0xa) {
        a->x = (g->timer << 8) / 10 - 0x100;
        a->y = 0x20;
        AnimatorSprite_ProcessAnimation(a, 0, 0);
    } else if (g->timer < 0x14) {
        AnimatorSprite_ProcessAnimation(a, 0, 0);
    } else if (g->timer >= 0x14) {
        b->x = 0;
        b->y = 0x20;
        AnimatorSprite_ProcessAnimation(b, 0, 0);
        g->unk30++;
    }
    if (g->timer >= 0) {
        if (g->timer > 0x5c)
            g->unk34 = 0x20;
        else
            g->unk34++;
    }
    if (g->unk18 != NULL)
        SpecialStagePlayer_Update(g->unk18);
    SpecialStageCamera_Update();
    if (g->timer == 0x5c) {
        if (!(gTouchState.unk10 & 0x40))
            return;
        Sound_PlaySeqArc(0, 0x10, 0x14);
    }
    switch (g->timer) {
    case 0x28:
        SpecialStageCamera_SetMode(0xa);
        break;
    case 0x5c:
        SpecialStageCamera_SetMode(0xb);
        break;
    case 0xf0:
        NNS_SndPlayerStopSeq(&gBGMSoundHandle, 0x3c);
        break;
    case 0x12c:
        SpecialStage_StartPlay();
        break;
    }
    g->timer++;
}
