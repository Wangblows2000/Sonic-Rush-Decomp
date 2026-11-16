#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x48 - 0x30];
} UnkSpr_022d4af8;

typedef struct {
    s32 timer;
    UnkSpr_022d4af8 sprB;
    UnkSpr_022d4af8 sprA;
    u32 animB;
    u32 animA;
} UnkWork_022d4af8;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022d4af8 *work;
} UnkTask_022d4af8;

typedef struct {
    u8 pad0[8];
    s16 bright;
} UnkBright_022d4af8;

extern UnkTask_022d4af8 *gTaskCurrent;
extern UnkBright_022d4af8 data_0208926c;
extern UnkBright_022d4af8 data_02087d44;

void AnimatorSprite_SetAnimation(UnkSpr_022d4af8 *s, u16 id);
void AnimatorSprite_ProcessAnimation(UnkSpr_022d4af8 *s, s32 a, s32 b);
void AnimatorSprite_Draw(UnkSpr_022d4af8 *s);
BOOL func_ov001_022d4960(void);
void func_ov001_022d44e4(UnkWork_022d4af8 *w);
void func_ov001_022d49a0(void);
void Task_DestroyAll(void);
void func_ov001_022d41c4(void);

#pragma thumb on
void func_ov001_022d4af8(void) {
    UnkWork_022d4af8 *w;
    s32 v;
    BOOL done;
    UnkSpr_022d4af8 *s;
    u32 *a;

    w = gTaskCurrent->work;
    if (w->timer >= 0x44) {
        if ((w->timer - 0x44) % 0x3c < 0x28) {
            s = &w->sprB;
            a = &w->animB;
            if (*a != 0) {
                *a = 0;
                AnimatorSprite_SetAnimation(s, *a);
            }
            s->flags &= ~4;
            AnimatorSprite_ProcessAnimation(s, 0, 0);
            AnimatorSprite_Draw(s);
        }
        s = &w->sprA;
        a = &w->animA;
        if (*a != 0) {
            *a = 0;
            AnimatorSprite_SetAnimation(s, *a);
        }
        s->flags &= ~4;
        AnimatorSprite_ProcessAnimation(s, 0, 0);
        AnimatorSprite_Draw(s);
    }
    if (w->timer >= 0x4a0) {
        v = 0x10 - (0x4b0 - w->timer);
        if (v > 0x10) v = 0x10;
        if (v < -0x10) v = -0x10;
        data_0208926c.bright = v;
        data_02087d44.bright = v;
    }
    done = FALSE;
    if (w->timer >= 0x44) {
        done = func_ov001_022d4960();
    }
    w->timer++;
    if (done == TRUE) {
        func_ov001_022d44e4(w);
        w->timer = 0;
        data_0208926c.bright = 0;
        data_02087d44.bright = 0;
        gTaskCurrent->func = func_ov001_022d49a0;
        return;
    }
    if (w->timer >= 0x4b0) {
        w->timer = 0;
        data_0208926c.bright = 0x10;
        data_02087d44.bright = 0x10;
        Task_DestroyAll();
        func_ov001_022d41c4();
    }
}
#pragma thumb reset
