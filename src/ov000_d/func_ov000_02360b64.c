#include <ov000_d/menuo.h>

#pragma thumb on

void func_ov000_02360b64(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr = &w->spr[17];
    s32 *cur = &w->anim[17];

    if (*cur != 3) {
        func_02033550(spr, 3);
        *cur = 3;
    }
    spr->unk2C &= ~4;
}

#pragma thumb reset
