#include <ov000_d/menuo.h>

#pragma thumb on

void func_ov000_02361068(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr = &w->spr78;
    s32 *cur = &w->anim6F4;

    if (*cur != 4) {
        func_02033550(spr, 4);
        *cur = 4;
    }
    spr->unk2C &= ~4;
    spr->unk2C &= ~1;
}

#pragma thumb reset
