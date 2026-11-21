#include <ov000_d/menuo.h>

#pragma thumb on

void TimeSelectMenu_SetSprite0Anim(UnkStruct_MenuO *w, s32 a) {
    UnkStruct_MenuOSpr *spr = &w->spr[0];
    s32 v = a + 5;
    s32 *cur = &w->anim[0];

    if (*cur != v) {
        AnimatorSprite_SetAnimation(spr, v);
        *cur = v;
    }
    spr->unk2C &= ~4;
    spr->unk2C &= ~1;
}

#pragma thumb reset
