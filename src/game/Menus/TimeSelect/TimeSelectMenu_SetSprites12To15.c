#include <game/Menus/menuo.h>

#pragma thumb on

void TimeSelectMenu_SetSprites12To15(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr;
    s32 *cur;

    spr = &w->spr[12];
    cur = &w->anim[12];
    if (*cur != 10) {
        AnimatorSprite_SetAnimation(spr, 10);
        *cur = 10;
    }
    spr->unk2C &= ~4;
    spr = &w->spr[13];
    cur = &w->anim[13];
    if (*cur != 11) {
        AnimatorSprite_SetAnimation(spr, 11);
        *cur = 11;
    }
    spr->unk2C &= ~4;
    spr = &w->spr[14];
    cur = &w->anim[14];
    if (*cur != 12) {
        AnimatorSprite_SetAnimation(spr, 12);
        *cur = 12;
    }
    spr->unk2C &= ~4;
    spr = &w->spr[15];
    cur = &w->anim[15];
    if (*cur != 13) {
        AnimatorSprite_SetAnimation(spr, 13);
        *cur = 13;
    }
    spr->unk2C &= ~4;
}

#pragma thumb reset
