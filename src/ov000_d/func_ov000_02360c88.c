#include <ov000_d/menuo.h>

#pragma thumb on

void func_ov000_02360c88(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr;
    s32 *cur;

    spr = &w->spr[8];
    cur = &w->anim[8];
    if (*cur != 6) {
        AnimatorSprite_SetAnimation(spr, 6);
        *cur = 6;
    }
    spr->unk2C &= ~4;
    spr = &w->spr[9];
    cur = &w->anim[9];
    if (*cur != 7) {
        AnimatorSprite_SetAnimation(spr, 7);
        *cur = 7;
    }
    spr->unk2C &= ~4;
    spr = &w->spr[10];
    cur = &w->anim[10];
    if (*cur != 8) {
        AnimatorSprite_SetAnimation(spr, 8);
        *cur = 8;
    }
    spr->unk2C &= ~4;
    spr = &w->spr[11];
    cur = &w->anim[11];
    if (*cur != 9) {
        AnimatorSprite_SetAnimation(spr, 9);
        *cur = 9;
    }
    spr->unk2C &= ~4;
}

#pragma thumb reset
