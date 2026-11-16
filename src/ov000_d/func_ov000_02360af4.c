#include <ov000_d/menuo.h>

#pragma thumb on

void func_ov000_02360af4(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr = &w->spr[19];
    s32 *cur = &w->anim[19];

    if (*cur != 0) {
        AnimatorSprite_SetAnimation(spr, 0);
        *cur = 0;
    }
    spr->unk2C &= ~4;
}

#pragma thumb reset
