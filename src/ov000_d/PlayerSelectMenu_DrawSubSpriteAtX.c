#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 unk4;
    s16 unk6;
} UnkSpr_02358108;

typedef struct {
    u8 pad0[0x5C4];
    UnkSpr_02358108 spr;
} UnkStruct_02358108;

typedef struct {
    s16 x;
    s16 y;
} UnkOfs_02358108;

extern UnkOfs_02358108 gDisplaySub[4];

void AnimatorSprite_ProcessAnimation(UnkSpr_02358108 *spr, s32 a, s32 b);
void AnimatorSprite_Draw(UnkSpr_02358108 *spr);

#pragma thumb on

void PlayerSelectMenu_DrawSubSpriteAtX(UnkStruct_02358108 *w, s16 x) {
    UnkSpr_02358108 *spr = &w->spr;

    spr->unk4 = x;
    spr->unk6 = 0x10;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    AnimatorSprite_Draw(spr);
    gDisplaySub[2].y = 0x20;
}

#pragma thumb reset
