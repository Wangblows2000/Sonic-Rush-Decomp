#include <ov000_d/menuo.h>

#pragma thumb on

void func_ov000_02360f98(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr = &w->sprC0;
    s32 *cur = &w->anim6F8;

    if (*cur != 3) {
        AnimatorSprite_SetAnimation(spr, 3);
        *cur = 3;
    }
    spr->unk2C |= 4;
    spr->unk2C &= ~1;
    spr->unk2C |= 0x80;
}

#pragma thumb reset
