#include <ov000_d/menuo.h>

#pragma thumb on

void TimeSelectMenu_SetSprite78Anim3Hidden(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr = &w->spr78;

    AnimatorSprite_SetAnimation(spr, 3);
    w->anim6F4 = 3;
    spr->unk2C |= 4;
    spr->unk2C &= ~1;
}

#pragma thumb reset
