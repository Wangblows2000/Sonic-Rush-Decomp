#include <ov000_d/sprobj.h>

#pragma thumb on

void func_ov000_0235def0(UnkStruct_SprObj *w, s32 sel, s32 scale) {
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
        func_02033550(spr, w->anim[10]);
    }
    func_0203344c(spr, 0, 0);
    spr->x = 4;
    spr->y = 0xc0;
    if (scale == 0x1000) {
        func_02032d2c(spr);
    } else {
        func_02032480(spr, 0x1000, scale, 0);
    }
    spr = &w->spr[11];
    if (w->anim[11] != b) {
        w->anim[11] = b;
        func_02033550(spr, w->anim[11]);
    }
    func_0203344c(spr, 0, 0);
    spr->x = 0x94;
    spr->y = 0xc0;
    if (scale == 0x1000) {
        func_02032d2c(spr);
    } else {
        func_02032480(spr, 0x1000, scale, 0);
    }
}

#pragma thumb reset
