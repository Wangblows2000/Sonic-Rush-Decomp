#include <game/Menus/menuo.h>

#pragma thumb on

void TimeSelectMenu_SetSprite30Anim0(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr = &w->spr30;
    s32 *cur = &w->anim6F0;

    if (*cur != 0) {
        AnimatorSprite_SetAnimation(spr, 0);
        *cur = 0;
    }
}

#pragma thumb reset
