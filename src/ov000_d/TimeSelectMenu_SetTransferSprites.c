#include <ov000_d/menuo.h>

#pragma thumb on

void TimeSelectMenu_SetTransferSprites(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr;
    s32 *cur;

    spr = &w->spr[3];
    cur = &w->anim[3];
    if (*cur != 5) {
        AnimatorSprite_SetAnimation(spr, 5);
        *cur = 5;
    }
    spr->unk2C &= ~4;
    spr = &w->spr[4];
    cur = &w->anim[4];
    if (*cur != 1) {
        AnimatorSprite_SetAnimation(spr, 1);
        *cur = 1;
    }
    spr->unk2C &= ~4;
    spr = &w->spr[5];
    cur = &w->anim[5];
    if (*cur != 2) {
        AnimatorSprite_SetAnimation(spr, 2);
        *cur = 2;
    }
    spr->unk2C |= 4;
}

#pragma thumb reset
