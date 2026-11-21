#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 unk4;
    s16 unk6;
} UnkSpr_023580bc;

typedef struct {
    u8 pad0[0x5C4];
    UnkSpr_023580bc spr;
} UnkStruct_023580bc;

typedef struct {
    u16 x;
    u16 y;
} UnkOfs_023580bc;

extern UnkOfs_023580bc gDisplaySub[4];

void AnimatorSprite_ProcessAnimation(UnkSpr_023580bc *spr, s32 a, s32 b);
void AnimatorSprite_Draw(UnkSpr_023580bc *spr);

#pragma thumb on

void PlayerSelectMenu_DrawSubTitleSprite2(UnkStruct_023580bc *w, BOOL flag) {
    UnkSpr_023580bc *spr = &w->spr;

    spr->unk4 = 0;
    spr->unk6 = 0x10;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    AnimatorSprite_Draw(spr);
    if (flag) {
        gDisplaySub[2].y = -8;
    } else {
        gDisplaySub[2].y = 0x20;
    }
}

#pragma thumb reset
