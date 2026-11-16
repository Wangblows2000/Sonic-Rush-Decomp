#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x2c - 8];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022d39ac;

typedef struct {
    u32 timer;
    s32 sel;
    u8 pad8[4];
    UnkSpr_022d39ac spr[6];
    s32 anim[6];
    u8 pad1d4[0x5f4 - 0x1d4];
    u32 vals[4];
    void (*fns[4])(void);
} UnkWork_022d39ac;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022d39ac *work;
} UnkTask_022d39ac;

extern UnkTask_022d39ac *gTaskCurrent;

void AnimatorSprite_SetAnimation(UnkSpr_022d39ac *s, u16 id);
void AnimatorSprite_ProcessAnimation(UnkSpr_022d39ac *s, s32 a, s32 b);
void AnimatorSprite_Draw(UnkSpr_022d39ac *s);
void TryAgain_CreateFadeTask(s32 a, s32 b, s32 c, s32 d);
void TryAgain_State_Unk_ov001_022d3864(void);

#pragma thumb on
void TryAgain_State_Unk_ov001_022d39ac(void) {
    UnkWork_022d39ac *w;
    UnkSpr_022d39ac *s;
    s32 i;
    u16 id;

    w = gTaskCurrent->work;
    s = &w->spr[0];
    if (w->anim[0] != 0) {
        AnimatorSprite_SetAnimation(s, 0);
        w->anim[0] = 0;
    }
    s->flags &= ~4;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
    for (i = 0; i < 4; i++) {
        if (w->sel == i) {
            s = &w->spr[1];
            if (w->anim[1] != 0) {
                AnimatorSprite_SetAnimation(s, 0);
                w->anim[1] = 0;
            }
            s->flags &= ~4;
            s->x = 0x40;
            s->y = i * 0x20 + 0x24;
            AnimatorSprite_ProcessAnimation(s, 0, 0);
            AnimatorSprite_Draw(s);
        }
        s = &w->spr[2 + i];
        id = w->vals[i];
        if (w->sel == i) {
            id += 2;
            s->flags |= 4;
        } else {
            s->flags &= ~4;
        }
        if (id != w->anim[2 + i]) {
            AnimatorSprite_SetAnimation(s, id);
            w->anim[2 + i] = id;
        }
        AnimatorSprite_ProcessAnimation(s, 0, 0);
        AnimatorSprite_Draw(s);
    }
    w->timer++;
    if (w->timer >= 0x20) {
        w->timer = 0;
        TryAgain_CreateFadeTask(0, 0x10, 0x10, 0);
        gTaskCurrent->func = TryAgain_State_Unk_ov001_022d3864;
    }
}
#pragma thumb reset
