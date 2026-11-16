#include <ov000_d/menuo.h>

#pragma thumb on

void func_ov000_02360a4c(UnkStruct_MenuO *w, s16 x, s16 y, s32 scale) {
    UnkStruct_MenuOSpr *spr = &w->spr[0];

    spr->x = x;
    spr->y = y;
    func_0203344c(spr, 0, 0);
    if (scale == 0x1000) {
        func_02032d2c(spr);
    } else {
        func_02032480(spr, scale, scale, 0);
    }
}

#pragma thumb reset
