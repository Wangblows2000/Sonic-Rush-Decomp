#include <nitro/types.h>

void AnimatorSprite_ProcessAnimation(void *p, s32 a, s32 b);
void AnimatorSprite_Draw(void *p);

#pragma thumb on
void func_ov001_022d9cd4(u8 *p, u16 a, u16 b) {
    u8 *q = p + 0x330;
    *(u16 *)(q + 4) = a;
    *(u16 *)(q + 6) = b;
    AnimatorSprite_ProcessAnimation(q, 0, 0);
    AnimatorSprite_Draw(q);
}
#pragma thumb reset
