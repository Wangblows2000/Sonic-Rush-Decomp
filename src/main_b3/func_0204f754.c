#include <nitro/types.h>
#include <main_b3/w50.h>

extern UnkTask_W50 *data_022b4574;

void func_0203344c(UnkSpr_W50 *s, u32 a, u32 b);
void func_02032d2c(UnkSpr_W50 *s);
void func_02033550(UnkSpr_W50 *s, u16 anim);
BOOL func_0204f3dc(void);
BOOL func_0204f40c(void);
BOOL func_0204f3ac(void);
BOOL func_0204f43c(void);
void func_02051f90(void);
void func_02051f20(void);
void func_02052104(s32 a);
void func_02051ebc(void);
void func_0204fdc8(void);
void func_0204fd7c(void);

#pragma thumb on
void func_0204f754(void) {
    UnkWork_W50 *w = data_022b4574->work;
    UnkSpr_W50 *spr;
    s32 i;
    s32 anim;
    BOOL done;
    BOOL a;
    BOOL b;
    u16 pad;

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
        if (i == w->unk4 && w->unk8 == 2)
            anim++;
        if (anim != w->anim[2 + i]) {
            func_02033550(spr, anim);
            w->anim[2 + i] = anim;
        }
        spr->unk2C &= ~4;
        func_0203344c(spr, 0, 0);
        func_02032d2c(spr);
    }
    if (w->unk24 == 1) {
        spr = &w->spr[4];
        anim = 0;
        if (w->unk0 < 6) {
            anim++;
            spr->unk2C |= 4;
        } else {
            spr->unk2C &= ~4;
        }
        if (anim != w->anim[4]) {
            func_02033550(spr, anim);
            w->anim[2 + i] = anim;
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
        if (w->unk8 > 1) {
            func_0203344c(spr, 0, 0);
            func_02032d2c(spr);
        }
    }
    done = FALSE;
    a = FALSE;
    b = FALSE;
    pad = data_022503e0.trg;
    if (pad & 1)
        a = TRUE;
    if (pad & 2)
        b = TRUE;
    switch (w->unk24) {
    case 0:
        if (func_0204f3dc() || a) {
            done = TRUE;
            func_02051f90();
        }
        break;
    case 1:
        if (func_0204f40c()) {
            w->unk4 = 0;
            done = TRUE;
            if (w->unk14 & 1)
                func_02052104(0x12);
            else
                func_02051f90();
        } else if (func_0204f3ac()) {
            done = TRUE;
            w->unk4 = 1;
            func_02051f90();
        } else if (a) {
            done = TRUE;
            if (w->unk4 == 0 && (w->unk14 & 1))
                func_02052104(0x12);
            else
                func_02051f90();
        } else if (b) {
            done = TRUE;
            w->unk4 = 1;
            func_02051f20();
        }
        break;
    case 2:
        if (func_0204f3dc() || a) {
            done = TRUE;
            func_02051f90();
        }
        break;
    case 3:
        if (func_0204f43c() || a) {
            done = TRUE;
            func_02051f90();
        }
        break;
    case 4:
    case 5:
        break;
    }
    if (!done) {
        pad = data_022503e0.trg;
        if (pad & 0x20) {
            if (w->unk4 > 0) {
                w->unk4--;
                w->unk0 = 0;
                func_02051ebc();
            }
        } else if (pad & 0x10) {
            if (w->unk4 < w->unk8 - 1) {
                w->unk4++;
                w->unk0 = 0;
                func_02051ebc();
            }
        }
    }
    w->unk0++;
    if (done) {
        if (b || w->unk24 == 3)
            func_0204fdc8();
        else
            func_0204fd7c();
    } else if (w->unk48) {
        func_0204fdc8();
    }
}
#pragma thumb reset
