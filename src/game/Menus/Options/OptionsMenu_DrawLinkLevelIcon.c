#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x24];
    u32 flags;
    u8 pad30[0x14];
    u16 unk44;
} UnkSpr_022cbd60;

typedef struct {
    u8 pad0[0x18c8];
    UnkSpr_022cbd60 spr;
    u8 pad[0x1ba0 - 0x18c8 - sizeof(UnkSpr_022cbd60)];
    u32 anim;
} UnkStruct_022cbd60;

s32 Wireless_GetLinkLevel(UnkStruct_022cbd60 *p);
void AnimatorSprite_SetAnimation(UnkSpr_022cbd60 *s, u16 anim);
void AnimatorSprite_ProcessAnimation(UnkSpr_022cbd60 *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022cbd60 *s);

#pragma thumb on
void OptionsMenu_DrawLinkLevelIcon(UnkStruct_022cbd60 *p, s32 x, s32 y) {
    UnkSpr_022cbd60 *s = &p->spr;
    u32 *cur = &p->anim;
    u32 anim;

    switch (Wireless_GetLinkLevel(p)) {
    case 0:
        anim = 1;
        break;
    case 1:
        anim = 2;
        break;
    case 2:
        anim = 3;
        break;
    case 3:
        anim = 4;
        break;
    }
    if (anim != *cur) {
        AnimatorSprite_SetAnimation(s, anim);
        *cur = anim;
    }
    s->x = x;
    s->y = y;
    s->flags &= 0x60000;
    s->flags |= 0x20000;
    s->unk44 = 1;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}
#pragma thumb reset
