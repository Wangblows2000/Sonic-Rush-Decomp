#include <game/Menus/menuo.h>

#pragma thumb on

void TimeSelectMenu_SetSprite2Anim0(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr = &w->spr[2];
    s32 *cur = &w->anim[2];

    if (*cur != 0) {
        AnimatorSprite_SetAnimation(spr, 0);
        *cur = 0;
    }
    spr->unk2C &= ~4;
}

#pragma thumb reset
