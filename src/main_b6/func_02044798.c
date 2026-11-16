#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x3c];
    u16 prio;
    u16 pad46;
} UnkSpr_02044798;

typedef struct {
    u8 pad0[0x10];
    UnkSpr_02044798 spr[16];
    UnkSpr_02044798 extra;
} UnkWork_02044798;

typedef struct {
    s16 x;
    s16 y;
} UnkPos_02044798;

extern u16 data_02073264[];
extern UnkPos_02044798 data_02073274[];

void AnimatorSprite_Draw(UnkSpr_02044798 *s);

void func_02044798(UnkWork_02044798 *w, s32 n, u16 prio) {
    s32 i;

    for (i = 0; i < n; i++) {
        UnkSpr_02044798 *s = &w->spr[data_02073264[i]];
        s->x = data_02073274[i].x;
        s->y = data_02073274[i].y;
        s->prio = prio;
        AnimatorSprite_Draw(s);
    }
    AnimatorSprite_Draw(&w->extra);
}
