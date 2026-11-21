#include <ov000_d/sprobj.h>

#pragma thumb on

void ResultsScreen_DrawWinLoseSprites(UnkStruct_SprObj *w, s32 sel, s32 scale) {
    s32 a;
    s32 b;
    UnkStruct_SprObjSpr *spr;

    switch (sel) {
    case 0:
        a = 9;
        b = 10;
        break;
    case 1:
        a = 10;
        b = 9;
        break;
    case 2:
    default:
        a = 11;
        b = 11;
        break;
    }
    spr = &w->spr[10];
    if (w->anim[10] != a) {
        w->anim[10] = a;
        AnimatorSprite_SetAnimation(spr, w->anim[10]);
    }
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    spr->x = 4;
    spr->y = 0xc0;
    if (scale == 0x1000) {
        AnimatorSprite_Draw(spr);
    } else {
        AnimatorSprite_DrawRotScale(spr, 0x1000, scale, 0);
    }
    spr = &w->spr[11];
    if (w->anim[11] != b) {
        w->anim[11] = b;
        AnimatorSprite_SetAnimation(spr, w->anim[11]);
    }
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    spr->x = 0x94;
    spr->y = 0xc0;
    if (scale == 0x1000) {
        AnimatorSprite_Draw(spr);
    } else {
        AnimatorSprite_DrawRotScale(spr, 0x1000, scale, 0);
    }
}

#pragma thumb reset
