#include <ov000_d/sprobj.h>

void ResultsScreen_DrawPlayerName(UnkStruct_SprObj *w, s32 x, s32 y, s32 row);

#pragma thumb on

void func_ov000_0235e124(UnkStruct_SprObj *w) {
    UnkStruct_SprObjSpr *spr;

    ResultsScreen_DrawPlayerName(w, 0x1c, 0xb, 0);
    ResultsScreen_DrawPlayerName(w, 0xac, 0xb, 1);
    spr = &w->spr[0];
    if (w->anim[0] != 3) {
        w->anim[0] = 3;
        AnimatorSprite_SetAnimation(spr, w->anim[0]);
    }
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    spr->x = 8;
    spr->y = 0;
    AnimatorSprite_Draw(spr);
    spr->x = 0x98;
    spr->y = 0;
    AnimatorSprite_Draw(spr);
}

#pragma thumb reset
