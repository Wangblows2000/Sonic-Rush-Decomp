#include <ov000_d/sprobj.h>

#pragma thumb on

void func_ov000_0235e018(UnkStruct_SprObj *w) {
    UnkStruct_SprObjSpr *spr = &w->spr[1];

    if (w->anim[1] != 4) {
        w->anim[1] = 4;
        AnimatorSprite_SetAnimation(spr, w->anim[1]);
    }
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    spr->x = 8;
    spr->y = 0x88;
    AnimatorSprite_Draw(spr);
    spr->x = 0x98;
    spr->y = 0x88;
    AnimatorSprite_Draw(spr);
}

#pragma thumb reset
