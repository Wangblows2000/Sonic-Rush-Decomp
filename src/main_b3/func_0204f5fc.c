#include <nitro/types.h>
#include <main_b3/w50.h>

extern UnkTask_W50 *data_022b4574;

void func_0203344c(UnkSpr_W50 *s, u32 a, u32 b);
void func_02032d2c(UnkSpr_W50 *s);
void func_02033550(UnkSpr_W50 *s, u16 anim);
void func_0204fdc8(void);

#pragma thumb on
void func_0204f5fc(void) {
    UnkWork_W50 *w = data_022b4574->work;
    UnkSpr_W50 *spr;
    s32 i;
    s32 anim;

    if (w->unk20 != 3) {
        spr = &w->spr[0];
        func_0203344c(spr, 0, 0);
        func_02032d2c(spr);
    }
    if (w->unk1C != 0x1c) {
        spr = &w->spr[1];
        func_0203344c(spr, 0, 0);
        func_02032d2c(spr);
    }
    for (i = 0; i < w->unk8; i++) {
        spr = &w->spr[2 + i];
        anim = w->unkC[i];
        if (i == w->unk4) {
            if (w->unk8 == 1)
                anim += 1;
            else
                anim += 2;
            spr->unk2C |= 4;
        } else {
            spr->unk2C &= ~4;
        }
        if (anim != w->anim[2 + i]) {
            func_02033550(spr, anim);
            w->anim[2 + i] = anim;
        }
        func_0203344c(spr, 0, 0);
        func_02032d2c(spr);
    }
    if (w->unk24 == 1) {
        spr = &w->spr[4];
        if (w->anim[4] != 0) {
            func_02033550(spr, 0);
            w->anim[2 + i] = 0;
        }
        if (w->unk8 == 1) {
            spr->x = 0x60;
            spr->y = 0x6c;
        } else if (w->unk4 == 0) {
            spr->x = 0x3c;
            spr->y = 0x68;
        } else {
            spr->x = 0x7c;
            spr->y = 0x68;
        }
        spr->unk2C &= ~4;
        if (w->unk8 > 1) {
            func_0203344c(spr, 0, 0);
            func_02032d2c(spr);
        }
    }
    w->unk0++;
    if (w->unk0 >= 0x20)
        func_0204fdc8();
}
#pragma thumb reset
