#include <ov000_d/sprobj.h>

#pragma thumb on

void func_ov000_0235ddec(UnkStruct_SprObj *w, BOOL flag) {
    s32 i;
    s32 a;
    UnkStruct_SprObjSpr *spr;
    s32 *cur;

    for (i = 0; i < 3; i++) {
        if (i == w->unk8) {
            spr = &w->spr[12];
            cur = &w->anim[12];
            if (w->unk4 < 6) {
                a = 1;
                spr->unk2C |= 4;
            } else {
                a = 0;
                spr->unk2C &= ~4;
            }
            if (*cur != a) {
                *cur = a;
                func_02033550(spr, *cur);
            }
            spr->x = 0x30;
            spr->y = 0x28 + i * 0x20;
            func_0203344c(spr, 0, 0);
            func_02032d2c(spr);
        }
        spr = &w->spr[13 + i];
        cur = &w->anim[13 + i];
        a = i * 3;
        if (i == w->unk8) {
            if (flag) {
                a += 2;
                spr->unk2C |= 4;
            } else {
                a += 1;
                spr->unk2C &= ~4;
            }
        } else {
            spr->unk2C &= ~4;
        }
        if (*cur != a) {
            *cur = a;
            func_02033550(spr, *cur);
        }
        func_0203344c(spr, 0, 0);
        func_02032d2c(spr);
    }
}

#pragma thumb reset
