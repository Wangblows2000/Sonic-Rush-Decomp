#include <nitro/types.h>

typedef struct UnkSpr_022ed3e0 {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkSpr_022ed3e0;

typedef struct UnkStruct_022ed3e0 {
    u8 pad0[0x4e4];
    UnkSpr_022ed3e0 spr[5];
    u8 pad64c[0x68c - 0x64c];
    u32 anim[5];
} UnkStruct_022ed3e0;

void AnimatorSprite_SetAnimation(UnkSpr_022ed3e0 *s, u16 anim);
void AnimatorSprite_ProcessAnimation(UnkSpr_022ed3e0 *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022ed3e0 *s);

#pragma thumb on
void func_ov001_022ed3e0(UnkStruct_022ed3e0 *p) {
    UnkSpr_022ed3e0 *s;
    u32 *anim;

    s = &p->spr[0];
    anim = &p->anim[0];
    if (*anim != 1) {
        *anim = 1;
        AnimatorSprite_SetAnimation(s, *anim);
    }
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);

    s = &p->spr[1];
    anim = &p->anim[1];
    if (*anim != 2) {
        *anim = 2;
        AnimatorSprite_SetAnimation(s, *anim);
    }
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);

    s = &p->spr[2];
    anim = &p->anim[2];
    if (*anim != 3) {
        *anim = 3;
        AnimatorSprite_SetAnimation(s, *anim);
    }
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}
#pragma thumb reset
