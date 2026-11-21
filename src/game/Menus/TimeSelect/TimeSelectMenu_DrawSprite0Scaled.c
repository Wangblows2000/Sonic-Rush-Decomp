#include <game/Menus/menuo.h>

#pragma thumb on

void TimeSelectMenu_DrawSprite0Scaled(UnkStruct_MenuO *w, s16 x, s16 y, s32 scale) {
    UnkStruct_MenuOSpr *spr = &w->spr[0];

    spr->x = x;
    spr->y = y;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    if (scale == 0x1000) {
        AnimatorSprite_Draw(spr);
    } else {
        AnimatorSprite_DrawRotScale(spr, scale, scale, 0);
    }
}

#pragma thumb reset
