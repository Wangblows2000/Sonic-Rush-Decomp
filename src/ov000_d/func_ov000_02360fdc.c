#include <ov000_d/menuo.h>

#pragma thumb on

void func_ov000_02360fdc(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr = &w->sprC0;
    s32 *cur = &w->anim6F8;

    if (*cur != 2) {
        func_02033550(spr, 2);
        *cur = 2;
    }
    spr->unk2C |= 4;
    spr->unk2C &= ~1;
    spr->unk2C |= 0x80;
}

#pragma thumb reset
