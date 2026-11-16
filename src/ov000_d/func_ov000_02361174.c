#include <ov000_d/menuo.h>

#pragma thumb on

void func_ov000_02361174(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr = &w->spr30;
    s32 *cur = &w->anim6F0;

    if (*cur != 0) {
        func_02033550(spr, 0);
        *cur = 0;
    }
}

#pragma thumb reset
