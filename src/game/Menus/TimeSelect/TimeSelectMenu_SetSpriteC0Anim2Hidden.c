#include <game/Menus/menuo.h>

#pragma thumb on

void TimeSelectMenu_SetSpriteC0Anim2Hidden(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr = &w->sprC0;
    s32 *cur = &w->anim6F8;

    if (*cur != 2) {
        AnimatorSprite_SetAnimation(spr, 2);
        *cur = 2;
    }
    spr->unk2C |= 4;
    spr->unk2C &= ~1;
    spr->unk2C |= 0x80;
}

#pragma thumb reset
