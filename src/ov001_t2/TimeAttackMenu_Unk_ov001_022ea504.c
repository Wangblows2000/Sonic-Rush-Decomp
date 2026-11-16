#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
} UnkSpr_022ea504;

typedef struct {
    u8 pad0[4];
    s32 mode;
    u8 pad8[0x930 - 8];
    UnkSpr_022ea504 spr0;
    u8 pad960[0xa98 - 0x930 - sizeof(UnkSpr_022ea504)];
    UnkSpr_022ea504 spr1;
    u8 padac8[0xb44 - 0xa98 - sizeof(UnkSpr_022ea504)];
    u32 anim0;
    u8 padb48[0xb58 - 0xb48];
    u32 anim1;
} UnkStruct_022ea504;

void AnimatorSprite_SetAnimation(UnkSpr_022ea504 *s, u16 anim);

#pragma thumb on
void TimeAttackMenu_Unk_ov001_022ea504(UnkStruct_022ea504 *p, s32 a, s32 b) {
    u16 anim;

    if (a >= 7) {
        anim = b * 2;
        if (p->anim1 != anim) {
            AnimatorSprite_SetAnimation(&p->spr1, anim);
            p->anim1 = anim;
        }
        p->spr1.flags &= ~4;
        return;
    }
    switch (p->mode) {
    case 0:
    case 2:
        anim = a * 4 + b * 2;
        break;
    case 1:
    case 3:
        anim = a * 2;
        break;
    }
    if (p->anim0 != anim) {
        AnimatorSprite_SetAnimation(&p->spr0, anim);
        p->anim0 = anim;
    }
    p->spr0.flags &= ~4;
}
#pragma thumb reset
