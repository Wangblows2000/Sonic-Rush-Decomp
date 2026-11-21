#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x48 - 0x30];
} UnkSprite_022d9f6c;

typedef struct {
    u8 pad0[0x210];
    UnkSprite_022d9f6c spr[31];
    u32 anim[31];
} UnkStruct_022d9f6c;

void AnimatorSprite_SetAnimation(void *s, s32 id);

#pragma thumb on
static inline void SetAnim_022d9f6c(UnkStruct_022d9f6c *p, s32 k, s32 v) {
    UnkSprite_022d9f6c *spr = &p->spr[k];
    AnimatorSprite_SetAnimation(spr, v);
    p->anim[k] = v;
    spr->flags &= ~4;
}

void VsMenu_Unk_ov001_022d9f6c(UnkStruct_022d9f6c *p) {
    SetAnim_022d9f6c(p, 11, 4);
}
#pragma thumb reset
