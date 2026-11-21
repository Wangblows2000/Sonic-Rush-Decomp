#include <ov000_d/menuo.h>

#pragma thumb on

void TimeSelectMenu_DrawTransferSprites(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr;

    spr = &w->spr[4];
    spr->x = 0x50;
    spr->y = 8;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    AnimatorSprite_Draw(spr);
    spr = &w->spr[5];
    spr->x = 0x98;
    spr->y = 0xc;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    AnimatorSprite_Draw(spr);
    spr = &w->spr[3];
    spr->x = 0;
    spr->y = 0;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    AnimatorSprite_Draw(spr);
}

#pragma thumb reset
