#include <game/Menus/menuo.h>

#pragma thumb on

void TimeSelectMenu_SetSprite7Anim1(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr = &w->spr[7];
    s32 *cur = &w->anim[7];

    if (*cur != 1) {
        AnimatorSprite_SetAnimation(spr, 1);
        *cur = 1;
    }
    spr->unk2C &= ~4;
}

#pragma thumb reset
