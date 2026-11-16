#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x48 - 0x30];
} UnkSprite_022d9ef4;

typedef struct {
    u8 pad0[0x210];
    UnkSprite_022d9ef4 spr[31];
    u32 anim[31];
} UnkStruct_022d9ef4;

void AnimatorSprite_SetAnimation(void *s, s32 id);

#pragma thumb on
static inline void SetAnim_022d9ef4(UnkStruct_022d9ef4 *p, s32 k, s32 v) {
    UnkSprite_022d9ef4 *spr = &p->spr[k];
    AnimatorSprite_SetAnimation(spr, v);
    p->anim[k] = v;
    spr->flags |= 4;
}

void func_ov001_022d9ef4(UnkStruct_022d9ef4 *p) {
    SetAnim_022d9ef4(p, 9, 6);
}
#pragma thumb reset
