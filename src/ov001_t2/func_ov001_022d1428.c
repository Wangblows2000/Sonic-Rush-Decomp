#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x40];
} UnkSpr_022d1428;

typedef struct {
    s32 state;
    u8 pad4[0xbe4 - 4];
    UnkSpr_022d1428 spr[3];
    s32 x[3];
    s32 y[3];
} UnkStruct_022d1428;

void AnimatorSprite_ProcessAnimation(UnkSpr_022d1428 *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022d1428 *s);

#pragma thumb on
void func_ov001_022d1428(UnkStruct_022d1428 *p, BOOL anim) {
    s32 i;

    if (p->state == 2) {
        return;
    }
    for (i = 0; i < 3; i++) {
        p->spr[i].x = p->x[i] >> 12;
        p->spr[i].y = p->y[i] >> 12;
        if (anim) {
            AnimatorSprite_ProcessAnimation(&p->spr[i], 0, 0);
        }
        AnimatorSprite_Draw(&p->spr[i]);
    }
}
#pragma thumb reset
