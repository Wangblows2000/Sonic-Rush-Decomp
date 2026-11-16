#include <ov000_d/sprobj.h>

#pragma thumb on

void func_ov000_0235ddac(UnkStruct_SprObj *w) {
    UnkStruct_SprObjSpr *spr = &w->spr[16];

    if (w->anim[16] != 12) {
        w->anim[16] = 12;
        func_02033550(spr, w->anim[16]);
    }
    func_0203344c(spr, 0, 0);
    func_02032d2c(spr);
}

#pragma thumb reset
