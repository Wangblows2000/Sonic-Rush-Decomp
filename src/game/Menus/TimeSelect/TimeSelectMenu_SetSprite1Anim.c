#include <game/Menus/menuo.h>

#pragma thumb on

void TimeSelectMenu_SetSprite1Anim(UnkStruct_MenuO *w, s32 a, s32 b) {
    UnkStruct_MenuOSpr *spr = &w->spr[1];
    s32 v = (a * 2 + b) * 2;
    s32 *cur = &w->anim[1];

    if (*cur != v) {
        AnimatorSprite_SetAnimation(spr, v);
        *cur = v;
    }
    spr->unk2C &= ~4;
}

#pragma thumb reset
