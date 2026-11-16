#include <ov000_d/sprobj.h>

#pragma thumb on

void func_ov000_0235e018(UnkStruct_SprObj *w) {
    UnkStruct_SprObjSpr *spr = &w->spr[1];

    if (w->anim[1] != 4) {
        w->anim[1] = 4;
        func_02033550(spr, w->anim[1]);
    }
    func_0203344c(spr, 0, 0);
    spr->x = 8;
    spr->y = 0x88;
    func_02032d2c(spr);
    spr->x = 0x98;
    spr->y = 0x88;
    func_02032d2c(spr);
}

#pragma thumb reset
