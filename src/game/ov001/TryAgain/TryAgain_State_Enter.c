#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x24];
    u32 flags;
} UnkSpr_022d3cd0;

typedef struct {
    s32 timer;
    s32 unk4;
    s32 unk8;
    UnkSpr_022d3cd0 spr;
    u8 pad[0x1bc - 0xc - sizeof(UnkSpr_022d3cd0)];
    s32 anim;
} UnkWork_022d3cd0;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
    u8 padc[4];
    UnkWork_022d3cd0 *work;
} UnkTask_022d3cd0;

extern UnkTask_022d3cd0 *gTaskCurrent;

void TryAgain_UpdateSplitScroll(UnkWork_022d3cd0 *w);
s16 TryAgain_Interpolate(s32 a, s32 b, s32 c, s32 t, s32 e);
BOOL TryAgain_IsSplitScrollDone(UnkWork_022d3cd0 *w);
void AnimatorSprite_SetAnimation(UnkSpr_022d3cd0 *s, u16 anim);
void AnimatorSprite_ProcessAnimation(UnkSpr_022d3cd0 *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022d3cd0 *s);
void TryAgain_State_Select(void);

#pragma thumb on
void TryAgain_State_Enter(void) {
    UnkWork_022d3cd0 *w = gTaskCurrent->work;
    UnkSpr_022d3cd0 *s;

    TryAgain_UpdateSplitScroll(w);
    s = &w->spr;
    if (w->anim != 0) {
        AnimatorSprite_SetAnimation(s, 0);
        w->anim = 0;
    }
    s->flags &= ~4;
    s->x = TryAgain_Interpolate(-0xa0, 0, 0xc, w->timer, 0x2000);
    s->y = 0;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
    w->timer++;
    if (TryAgain_IsSplitScrollDone(w)) {
        w->timer = 0;
        w->unk4 = 0;
        w->unk8 = 7;
        gTaskCurrent->fn = TryAgain_State_Select;
    }
}
#pragma thumb reset
