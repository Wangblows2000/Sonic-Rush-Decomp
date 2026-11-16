#include <ov000_d/menuo.h>

#pragma thumb on

void func_ov000_02360998(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr;

    spr = &w->spr[4];
    spr->x = 0x50;
    spr->y = 8;
    func_0203344c(spr, 0, 0);
    func_02032d2c(spr);
    spr = &w->spr[5];
    spr->x = 0x98;
    spr->y = 0xc;
    func_0203344c(spr, 0, 0);
    func_02032d2c(spr);
    spr = &w->spr[3];
    spr->x = 0;
    spr->y = 0;
    func_0203344c(spr, 0, 0);
    func_02032d2c(spr);
}

#pragma thumb reset
