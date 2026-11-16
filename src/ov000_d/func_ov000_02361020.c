#include <ov000_d/menuo.h>

#pragma thumb on

void func_ov000_02361020(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr = &w->sprC0;

    func_02033550(spr, 2);
    w->anim6F8 = 2;
    func_0203344c(spr, 0, 0);
    spr->unk2C |= 4;
    spr->unk2C |= 1;
    spr->unk2C |= 0x80;
}

#pragma thumb reset
