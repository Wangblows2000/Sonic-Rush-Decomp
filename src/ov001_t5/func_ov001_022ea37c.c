#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s32 mode;
    u8 pad8[0x8e8 - 8];
    u8 spr[0x914 - 0x8e8];
    u32 flags;
    u8 pad918[0xb40 - 0x918];
    s32 anim;
} UnkStruct_022ea37c;

void AnimatorSprite_SetAnimation(void *s, s32 id);

#pragma thumb on
void func_ov001_022ea37c(UnkStruct_022ea37c *p) {
    s32 id;

    switch (p->mode) {
    case 0:
        id = 0;
        break;
    case 2:
        id = 1;
        break;
    case 1:
        id = 0;
        break;
    case 3:
        id = 1;
        break;
    }
    if (p->anim != id) {
        AnimatorSprite_SetAnimation(p->spr, id);
        p->anim = id;
    }
    p->flags &= ~4;
}
#pragma thumb reset
