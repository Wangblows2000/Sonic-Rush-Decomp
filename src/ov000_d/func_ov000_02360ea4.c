#include <ov000_d/menuo.h>

#pragma thumb on

void func_ov000_02360ea4(UnkStruct_MenuO *w, s32 a, s32 b) {
    UnkStruct_MenuOSpr *spr = &w->spr[1];
    s32 v = (a * 2 + b) * 2 + 1;
    s32 *cur = &w->anim[1];

    if (*cur != v) {
        func_02033550(spr, v);
        *cur = v;
    }
    spr->unk2C &= ~4;
}

#pragma thumb reset
