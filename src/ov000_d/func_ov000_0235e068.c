#include <ov000_d/sprobj.h>

#pragma thumb on

void func_ov000_0235e068(UnkStruct_SprObj *w) {
    UnkStruct_SprObjSpr *spr;
    s32 *cur;
    s32 a;
    s32 i;

    spr = &w->spr[2];
    cur = &w->anim[2];
    if (w->unk18 == 0) {
        a = 6;
    } else {
        a = 10;
    }
    for (i = 0; i < 4; i++) {
        if (*cur != a) {
            *cur = a;
            func_02033550(spr, *cur);
        }
        spr->x = 0x38;
        spr->y = 0x58;
        func_0203344c(spr, 0, 0);
        func_02032d2c(spr);
        spr++;
        cur++;
        a++;
    }
    spr = &w->spr[6];
    cur = &w->anim[6];
    if (w->unk1C == 0) {
        a = 6;
    } else {
        a = 10;
    }
    for (i = 0; i < 4; i++) {
        if (*cur != a) {
            *cur = a;
            func_02033550(spr, *cur);
        }
        spr->x = 0xc8;
        spr->y = 0x58;
        func_0203344c(spr, 0, 0);
        func_02032d2c(spr);
        spr++;
        cur++;
        a++;
    }
}

#pragma thumb reset
