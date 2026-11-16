#include <nitro/types.h>

typedef struct UnkSpr_022ed490 {
    u8 pad0[4];
    s16 x;
    s16 y;
} UnkSpr_022ed490;

typedef struct UnkStruct_022ed490 {
    u8 pad0[0x5bc];
    UnkSpr_022ed490 spr;
    u8 pad[0x698 - 0x5c4];
    u32 anim;
} UnkStruct_022ed490;

void AnimatorSprite_SetAnimation(UnkSpr_022ed490 *s, u16 anim);
void AnimatorSprite_ProcessAnimation(UnkSpr_022ed490 *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022ed490 *s);

#pragma thumb on
void func_ov001_022ed490(UnkStruct_022ed490 *p, s16 x, s16 y) {
    UnkSpr_022ed490 *s = &p->spr;
    u32 *anim = &p->anim;
    if (*anim != 0) {
        *anim = 0;
        AnimatorSprite_SetAnimation(s, *anim);
    }
    s->x = x;
    s->y = y;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}
#pragma thumb reset
