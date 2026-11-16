#include <ov000_d/menuo.h>

#pragma thumb on

void func_ov000_02360e6c(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr = &w->spr[2];
    s32 *cur = &w->anim[2];

    if (*cur != 0) {
        func_02033550(spr, 0);
        *cur = 0;
    }
    spr->unk2C &= ~4;
}

#pragma thumb reset
