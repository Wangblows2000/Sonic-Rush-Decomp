#include <ov000_d/menuo.h>

#pragma thumb on

void func_ov000_02360a94(UnkStruct_MenuO *w, s16 x, s16 y) {
    UnkStruct_MenuOSpr *spr = &w->sprC0;

    spr->x = x;
    spr->y = y;
    func_0203344c(spr, 0, 0);
    func_02032d2c(spr);
}

#pragma thumb reset
