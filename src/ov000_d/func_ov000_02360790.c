#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkSpr_02360790;

typedef struct {
    u8 pad0[0x468];
    UnkSpr_02360790 spr[4];
} UnkStruct_02360790;

void func_0203344c(UnkSpr_02360790 *spr, s32 a, s32 b);
void func_02032d2c(UnkSpr_02360790 *spr);
void func_02032480(UnkSpr_02360790 *spr, s32 sx, s32 sy, s32 c);

#pragma thumb on

void func_ov000_02360790(UnkStruct_02360790 *w, s32 x, s32 y, s32 scale) {
    UnkSpr_02360790 *s0;
    UnkSpr_02360790 *s2;
    UnkSpr_02360790 *s1;

    s0 = &w->spr[0];
    s0->x = x;
    s0->y = y;
    func_0203344c(s0, 0, 0);
    if (scale == 0x1000) {
        func_02032d2c(s0);
    } else {
        func_02032480(s0, scale, scale, 0);
    }
    s1 = &w->spr[1];
    s1->x = x;
    s1->y = y;
    func_0203344c(s1, 0, 0);
    if (scale == 0x1000) {
        func_02032d2c(s1);
    } else {
        func_02032480(s1, scale, scale, 0);
    }
    s2 = &w->spr[2];
    s2->x = x;
    s2->y = y;
    func_0203344c(s2, 0, 0);
    if (scale == 0x1000) {
        func_02032d2c(s2);
    } else {
        func_02032480(s2, scale, scale, 0);
    }
    w = (UnkStruct_02360790 *)&w->spr[3];
    ((UnkSpr_02360790 *)w)->x = x;
    ((UnkSpr_02360790 *)w)->y = y;
    func_0203344c((UnkSpr_02360790 *)w, 0, 0);
    if (scale == 0x1000) {
        func_02032d2c((UnkSpr_02360790 *)w);
    } else {
        func_02032480((UnkSpr_02360790 *)w, scale, scale, 0);
    }
}

#pragma thumb reset
