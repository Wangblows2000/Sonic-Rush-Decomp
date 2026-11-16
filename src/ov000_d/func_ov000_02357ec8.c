#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 unk4;
    s16 unk6;
    u8 pad8[0x48 - 8];
} UnkSpr_02357ec8;

typedef struct {
    u8 pad0[0x84C];
    UnkSpr_02357ec8 spr[4];
    u8 pad96C[0xB40 - 0x96C];
    s32 anim[4];
} UnkStruct_02357ec8;

void func_02033550(UnkSpr_02357ec8 *spr, u16 anim);
void func_0203344c(UnkSpr_02357ec8 *spr, s32 a, s32 b);
void func_02032480(UnkSpr_02357ec8 *spr, s32 sx, s32 sy, s32 c);
void func_02032d2c(UnkSpr_02357ec8 *spr);

#pragma thumb on

void func_ov000_02357ec8(UnkStruct_02357ec8 *w, s32 sel, s32 scale) {
    UnkSpr_02357ec8 *spr;
    s32 a0;
    s32 a1;
    s32 a2;
    s32 a3;

    if (scale <= 0) {
        return;
    }
    switch (sel) {
    case 0:
        a0 = 0x11;
        a1 = 0x12;
        a2 = 0x13;
        a3 = 0x14;
        break;
    case 1:
        a0 = 0x15;
        a1 = 0x16;
        a2 = 0x17;
        a3 = 0x18;
        break;
    case 2:
        a0 = 0x19;
        a1 = 0x1a;
        a2 = 0x1b;
        a3 = 0x1c;
        break;
    default:
        return;
    }
    spr = &w->spr[0];
    if (w->anim[0] != a0) {
        func_02033550(spr, a0);
        w->anim[0] = a0;
    }
    spr->unk4 = 0x80;
    spr->unk6 = 0x58;
    func_0203344c(spr, 0, 0);
    if (scale != 0x1000) {
        func_02032480(spr, scale, scale, 0);
    } else {
        func_02032d2c(spr);
    }
    spr = &w->spr[1];
    if (w->anim[1] != a1) {
        func_02033550(spr, a1);
        w->anim[1] = a1;
    }
    spr->unk4 = 0x80;
    spr->unk6 = 0x58;
    func_0203344c(spr, 0, 0);
    if (scale != 0x1000) {
        func_02032480(spr, scale, scale, 0);
    } else {
        func_02032d2c(spr);
    }
    spr = &w->spr[2];
    if (w->anim[2] != a2) {
        func_02033550(spr, a2);
        w->anim[2] = a2;
    }
    spr->unk4 = 0x80;
    spr->unk6 = 0x58;
    func_0203344c(spr, 0, 0);
    if (scale != 0x1000) {
        func_02032480(spr, scale, scale, 0);
    } else {
        func_02032d2c(spr);
    }
    spr = &w->spr[3];
    if (w->anim[3] != a3) {
        func_02033550(spr, a3);
        w->anim[3] = a3;
    }
    spr->unk4 = 0x80;
    spr->unk6 = 0x58;
    func_0203344c(spr, 0, 0);
    if (scale != 0x1000) {
        func_02032480(spr, scale, scale, 0);
    } else {
        func_02032d2c(spr);
    }
}

#pragma thumb reset
