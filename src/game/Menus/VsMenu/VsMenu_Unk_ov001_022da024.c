#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022da024;

typedef struct {
    u8 pad0[0x210];
    UnkSpr_022da024 spr[31];
    s32 anim[31];
} UnkStruct_022da024;

void AnimatorSprite_SetAnimation(UnkSpr_022da024 *s, u16 anim);

#pragma thumb on
static inline void SetAnimOff_022da024(UnkStruct_022da024 *p, s32 k, s32 v) {
    UnkSpr_022da024 *spr = &p->spr[k];
    if (p->anim[k] != v) {
        AnimatorSprite_SetAnimation(spr, v);
        p->anim[k] = v;
    }
    spr->flags &= ~4;
}

void VsMenu_Unk_ov001_022da024(UnkStruct_022da024 *p) {
    SetAnimOff_022da024(p, 4, 0);
}
#pragma thumb reset
