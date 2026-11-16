#include <ov000_d/menuo.h>

#pragma thumb on

void func_ov000_02360e34(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr = &w->spr[2];
    s32 *cur = &w->anim[2];

    if (*cur != 1) {
        func_02033550(spr, 1);
        *cur = 1;
    }
    spr->unk2C &= ~4;
}

#pragma thumb reset
