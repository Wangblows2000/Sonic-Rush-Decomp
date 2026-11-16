#include <ov000_d/menuo.h>

#pragma thumb on

void func_ov000_0236114c(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr = &w->spr30;
    s32 *cur = &w->anim6F0;

    if (*cur != 1) {
        AnimatorSprite_SetAnimation(spr, 1);
        *cur = 1;
    }
}

#pragma thumb reset
