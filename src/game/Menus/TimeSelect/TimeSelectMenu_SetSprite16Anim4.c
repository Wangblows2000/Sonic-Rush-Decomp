#include <game/Menus/menuo.h>

#pragma thumb on

void TimeSelectMenu_SetSprite16Anim4(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr = &w->spr[16];
    s32 *cur = &w->anim[16];

    if (*cur != 4) {
        AnimatorSprite_SetAnimation(spr, 4);
        *cur = 4;
    }
    spr->unk2C &= ~4;
}

#pragma thumb reset
