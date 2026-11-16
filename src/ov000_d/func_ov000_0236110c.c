#include <ov000_d/menuo.h>

#pragma thumb on

void func_ov000_0236110c(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr = &w->spr78;

    func_02033550(spr, 2);
    w->anim6F4 = 2;
    func_0203344c(spr, 0, 0);
    spr->unk2C |= 4;
    spr->unk2C |= 1;
}

#pragma thumb reset
