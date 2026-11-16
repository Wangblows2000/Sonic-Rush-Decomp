#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022da288;

typedef struct {
    u8 pad0[0x210];
    UnkSpr_022da288 spr[31];
    s32 anim[31];
} UnkStruct_022da288;

void AnimatorSprite_SetAnimation(UnkSpr_022da288 *s, u16 anim);

#pragma thumb on
void VsMenu_Unk_ov001_022da288(UnkStruct_022da288 *p, s32 i) {
    s32 k;
    UnkSpr_022da288 *spr;
    s32 *cur;

    k = i + 2;
    spr = &p->spr[k];
    cur = &p->anim[k];
    if (*cur != 3) {
        AnimatorSprite_SetAnimation(spr, 3);
        *cur = 3;
    }
    spr->flags &= ~4;
}
#pragma thumb reset
