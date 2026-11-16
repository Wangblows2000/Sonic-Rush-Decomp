#include <nitro/types.h>

void AnimatorSprite_ProcessAnimation(void *p, s32 a, s32 b);
void AnimatorSprite_Draw(void *p);

void func_ov001_022c7a44(u8 *p) {
    u8 *q;
    q = p + 0x524;
    AnimatorSprite_ProcessAnimation(q, 0, 0);
    AnimatorSprite_Draw(q);
    q = p + 0x56c;
    AnimatorSprite_ProcessAnimation(q, 0, 0);
    AnimatorSprite_Draw(q);
    q = p + 0x5fc;
    AnimatorSprite_ProcessAnimation(q, 0, 0);
    AnimatorSprite_Draw(q);
}
