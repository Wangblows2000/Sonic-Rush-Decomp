#include <ov000_d/menuo.h>

#pragma thumb on

void TimeSelectMenu_SetSprite17Anim3B(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr = &w->spr[17];
    s32 *cur = &w->anim[17];

    if (*cur != 3) {
        AnimatorSprite_SetAnimation(spr, 3);
        *cur = 3;
    }
    spr->unk2C &= ~4;
}

#pragma thumb reset
