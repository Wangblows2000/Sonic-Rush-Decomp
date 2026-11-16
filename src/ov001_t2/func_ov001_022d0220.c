#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u16 anim;
} UnkSpr_022d0220;

typedef struct {
    u8 pad0[0xbe4];
    UnkSpr_022d0220 spr;
    u8 padbee[0xc10 - 0xbe4 - sizeof(UnkSpr_022d0220)];
    u32 flags;
} UnkStruct_022d0220;

void AnimatorSprite_SetAnimation(UnkSpr_022d0220 *s, u16 anim);

#pragma thumb on
void func_ov001_022d0220(UnkStruct_022d0220 *p) {
    if (p->flags & 2) {
        switch (p->spr.anim) {
        case 0x1a:
            AnimatorSprite_SetAnimation(&p->spr, 0x1b);
            p->flags &= ~4;
            break;
        case 0x1b:
            AnimatorSprite_SetAnimation(&p->spr, 0x1c);
            p->flags &= ~4;
            break;
        case 0x1c:
            AnimatorSprite_SetAnimation(&p->spr, 0x1d);
            p->flags |= 4;
            break;
        }
    }
}
#pragma thumb reset
