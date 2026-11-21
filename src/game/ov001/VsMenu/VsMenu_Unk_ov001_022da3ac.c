#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022da3ac;

typedef struct {
    u8 pad0[0x210];
    UnkSpr_022da3ac spr[31];
    s32 anim[31];
} UnkStruct_022da3ac;

void AnimatorSprite_SetAnimation(UnkSpr_022da3ac *s, u16 anim);

#pragma thumb on
void VsMenu_Unk_ov001_022da3ac(UnkStruct_022da3ac *p) {
    UnkSpr_022da3ac *spr = &p->spr[30];

    if (p->anim[30] != 4) {
        AnimatorSprite_SetAnimation(spr, 4);
        p->anim[30] = 4;
    }
    spr->flags &= ~4;
}
#pragma thumb reset
