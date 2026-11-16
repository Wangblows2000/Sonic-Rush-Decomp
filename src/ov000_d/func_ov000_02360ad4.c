#include <ov000_d/menuo.h>

#pragma thumb on

void func_ov000_02360ad4(UnkStruct_MenuO *w, s16 x, s16 y) {
    UnkStruct_MenuOSpr *spr = &w->spr30;

    spr->x = x;
    spr->y = y;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    AnimatorSprite_Draw(spr);
}

#pragma thumb reset
