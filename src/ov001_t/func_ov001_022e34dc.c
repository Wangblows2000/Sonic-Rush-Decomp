#include <nitro/types.h>

typedef struct {
    u8 pad0[0xa4];
    u8 *work;
} UnkStruct_022e34dc;

void AnimatorSprite_SetAnimation(void *s, u16 anim);

#pragma thumb on
void func_ov001_022e34dc(UnkStruct_022e34dc *p) {
    u8 *w = p->work + 0x1b08;

    AnimatorSprite_SetAnimation(w + 0x12c, 0);
    *(u32 *)(w + 0x1a0) &= ~4;
}
#pragma thumb reset
