#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022da374;

typedef struct {
    u8 pad0[0x210];
    UnkSpr_022da374 spr[31];
    s32 anim[31];
} UnkStruct_022da374;

void AnimatorSprite_SetAnimation(UnkSpr_022da374 *s, u16 anim);

#pragma thumb on
void VsMenu_Unk_ov001_022da374(UnkStruct_022da374 *p) {
    UnkSpr_022da374 *spr = &p->spr[30];

    if (p->anim[30] != 5) {
        AnimatorSprite_SetAnimation(spr, 5);
        p->anim[30] = 5;
    }
    spr->flags &= ~4;
}
#pragma thumb reset
