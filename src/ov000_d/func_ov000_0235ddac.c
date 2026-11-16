#include <ov000_d/sprobj.h>

#pragma thumb on

void func_ov000_0235ddac(UnkStruct_SprObj *w) {
    UnkStruct_SprObjSpr *spr = &w->spr[16];

    if (w->anim[16] != 12) {
        w->anim[16] = 12;
        AnimatorSprite_SetAnimation(spr, w->anim[16]);
    }
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    AnimatorSprite_Draw(spr);
}

#pragma thumb reset
