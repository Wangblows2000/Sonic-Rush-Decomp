#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022d9fa4;

typedef struct {
    u8 pad0[0x210];
    UnkSpr_022d9fa4 spr[31];
    s32 anim[31];
} UnkStruct_022d9fa4;

void AnimatorSprite_SetAnimation(UnkSpr_022d9fa4 *s, u16 anim);

#pragma thumb on
static inline void SetAnimOff_022d9fa4(UnkStruct_022d9fa4 *p, s32 k, s32 v) {
    UnkSpr_022d9fa4 *spr = &p->spr[k];
    if (p->anim[k] != v) {
        AnimatorSprite_SetAnimation(spr, v);
        p->anim[k] = v;
    }
    spr->flags &= ~4;
}

void func_ov001_022d9fa4(UnkStruct_022d9fa4 *p) {
    SetAnimOff_022d9fa4(p, 4, 9);
}
#pragma thumb reset
