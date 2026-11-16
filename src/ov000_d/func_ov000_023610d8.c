#include <ov000_d/menuo.h>

#pragma thumb on

void func_ov000_023610d8(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr = &w->spr78;

    AnimatorSprite_SetAnimation(spr, 2);
    w->anim6F4 = 2;
    spr->unk2C |= 4;
    spr->unk2C &= ~1;
}

#pragma thumb reset
