#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x2c - 8];
    u32 flags;
    u8 pad30[0x48 - 0x30];
} UnkSpr_022d3af0;

typedef struct {
    s32 timer2;
    s32 sel;
    u32 timer;
    UnkSpr_022d3af0 spr[6];
    s32 cur0;
    s32 cur1;
    s32 curs[4];
    u8 pad1d4[0x5f4 - 0x1d4];
    s32 base[4];
} UnkWork_022d3af0;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022d3af0 *work;
} UnkTask_022d3af0;

typedef struct {
    u8 pad0[4];
    u16 trg;
    u8 pad6[2];
    u16 rep;
} UnkPad_022d3af0;

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022d3af0;

extern UnkTask_022d3af0 *gTaskCurrent;
extern UnkPad_022d3af0 gPad;
extern UnkTouch_022d3af0 gTouchState;

void AnimatorSprite_SetAnimation(UnkSpr_022d3af0 *s, s32 anim);
void AnimatorSprite_ProcessAnimation(UnkSpr_022d3af0 *s, s32 a, s32 b);
void AnimatorSprite_Draw(UnkSpr_022d3af0 *s);
void SysSound_PlaySE(s32 se);
void TryAgain_State_Unk_ov001_022d39ac(void);

#pragma thumb on
void TryAgain_State_Unk_ov001_022d3af0(void) {
    UnkWork_022d3af0 *w;
    UnkSpr_022d3af0 *s;
    s32 i;
    s32 y;
    u16 f;
    BOOL done;
    s32 ry;
    s32 tx;
    s32 ty;
    s32 bot;

    w = gTaskCurrent->work;
    s = &w->spr[0];
    if (w->cur0 != 0) {
        AnimatorSprite_SetAnimation(s, 0);
        w->cur0 = 0;
    }
    s->flags &= ~4;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
    i = 0;
    y = 0x24;
    for (; i < 4; i++) {
        if (w->sel == i) {
            s = &w->spr[1];
            f = 0;
            if (w->timer < 7) {
                f++;
            }
            if (w->cur1 != f) {
                AnimatorSprite_SetAnimation(s, f);
                w->cur1 = f;
            }
            s->flags &= ~4;
            s->x = 0x40;
            s->y = y;
            AnimatorSprite_ProcessAnimation(s, 0, 0);
            AnimatorSprite_Draw(s);
        }
        s = &w->spr[i + 2];
        f = w->base[i];
        if (w->sel == i) {
            f++;
        }
        if (f != w->curs[i]) {
            AnimatorSprite_SetAnimation(s, f);
            w->curs[i] = f;
        }
        s->flags &= ~4;
        AnimatorSprite_ProcessAnimation(s, 0, 0);
        AnimatorSprite_Draw(s);
        y += 0x20;
    }
    w->timer2++;
    w->timer++;
    done = FALSE;
    if (gPad.rep & 0x40) {
        w->sel--;
        if (w->sel < 0) {
            w->sel = 3;
        }
        w->timer = 0;
        SysSound_PlaySE(4);
    } else if (gPad.rep & 0x80) {
        w->sel++;
        if (w->sel >= 4) {
            w->sel = 0;
        }
        w->timer = 0;
        SysSound_PlaySE(4);
    } else if (gPad.trg & 1) {
        done = TRUE;
    } else if (gTouchState.flags & 0x40) {
        tx = gTouchState.x;
        ty = gTouchState.y;
        for (i = 0, ry = 0x24; i < 4; ry += 0x20, i++) {
            bot = ry + 0x18;
            if (tx >= 0x40 && tx < 0xc0 && ty >= ry && ty < bot) {
                w->sel = i;
                w->timer = 0;
                done = TRUE;
            }
        }
    }
    if (done) {
        SysSound_PlaySE(0);
        w->timer2 = 0;
        gTaskCurrent->func = TryAgain_State_Unk_ov001_022d39ac;
    }
}
#pragma thumb reset
