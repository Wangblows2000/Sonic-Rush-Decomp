#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x40];
} UnkSpr_02063e10;

typedef struct {
    u8 pad0[0x10];
    UnkSpr_02063e10 spr[25];
    s16 sel0;
    s16 sel1;
    s32 unk71c;
} UnkWork_02063e10;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02063e10 *work;
} UnkTask_02063e10;

extern UnkTask_02063e10 *gTaskCurrent;

void AnimatorSprite_Draw(UnkSpr_02063e10 *s);

void func_02063e10(void) {
    UnkWork_02063e10 *w = gTaskCurrent->work;
    UnkSpr_02063e10 *s;

    AnimatorSprite_Draw(&w->spr[13]);
    w->spr[10].x = -2;
    AnimatorSprite_Draw(&w->spr[10]);
    if (w->sel0 != -1) {
        s = &w->spr[w->sel0];
        s->x = 0x39;
        s->y = 0x57;
        AnimatorSprite_Draw(s);
    }
    s = &w->spr[w->sel1];
    s->x = 0x43;
    s->y = 0x57;
    AnimatorSprite_Draw(s);
    if (w->unk71c != 0)
        AnimatorSprite_Draw(&w->spr[24]);
}
