#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022da064;

typedef struct {
    u8 pad0[0x210];
    UnkSpr_022da064 spr[31];
    s32 anim[31];
} UnkStruct_022da064;

void AnimatorSprite_SetAnimation(UnkSpr_022da064 *s, u16 anim);

#pragma thumb on
void VsMenu_Unk_ov001_022da064(UnkStruct_022da064 *p, s32 i) {
    s32 k;
    UnkSpr_022da064 *spr;
    s32 *cur;

    k = i + 5;
    spr = &p->spr[k];
    cur = &p->anim[k];
    if (*cur != 5) {
        AnimatorSprite_SetAnimation(spr, 5);
        *cur = 5;
    }
    spr->flags &= ~4;
    k = i + 7;
    spr = &p->spr[k];
    cur = &p->anim[k];
    if (*cur != 7) {
        AnimatorSprite_SetAnimation(spr, 7);
        *cur = 7;
    }
    spr->flags |= 4;
}
#pragma thumb reset
