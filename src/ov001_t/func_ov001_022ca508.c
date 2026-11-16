#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x3e];
    u16 unk46;
} UnkSpr_022ca508;

typedef struct {
    u32 unk0;
    s32 unk4;
} UnkCfg_022ca508;

typedef struct {
    u8 pad0[8];
    UnkCfg_022ca508 cfg;
    u8 pad10[0x408 - 0x10];
    UnkSpr_022ca508 spr[5];
    s32 anim[5];
} UnkWork_022ca508;

typedef struct {
    u8 pad0[0x14];
    u32 unk14;
    s32 unk18;
    s32 unk1c;
    u8 pad20[0x2c - 0x20];
    s32 unk2c;
    s32 unk30;
    s32 unk34;
    u8 pad38[0x3ac - 0x38];
    UnkWork_022ca508 *work;
} UnkStruct_022ca508;

void func_02033550(UnkSpr_022ca508 *s, u16 anim);
void func_0203344c(UnkSpr_022ca508 *s, u32 a, u32 b);
void func_02032d2c(UnkSpr_022ca508 *s);
void func_ov001_022ca2d4(UnkStruct_022ca508 *p, s32 x, s32 y, s32 a);
void func_ov001_022ca248(UnkStruct_022ca508 *p, s32 x, s32 y, s32 m);

#pragma thumb on
void func_ov001_022ca508(UnkStruct_022ca508 *p, s32 x, s32 y) {
    s32 ys;
    s32 xs;
    UnkWork_022ca508 *w;
    UnkSpr_022ca508 *spr;
    u16 a;
    s32 m;
    BOOL show;

    if (x >= 0x100) return;
    w = p->work;
    if (p->unk1c >= 0) {
        func_ov001_022ca2d4(p, x, y, 1);
    } else {
        func_ov001_022ca2d4(p, x, y, 0);
    }
    spr = &w->spr[0];
    switch (p->unk2c) {
    case 0:
        a = 0x19;
        break;
    case 1:
        a = 0x1a;
        break;
    }
    if (w->anim[0] != a) {
        func_02033550(spr, a);
        w->anim[0] = a;
    }
    xs = x + 0x18;
    spr->x = xs;
    spr->y = y + 0x18;
    func_0203344c(spr, 0, 0);
    func_02032d2c(spr);

    show = TRUE;
    if (p->unk34 != -1 && p->unk2c == 0 && p->unk30 == 0 && p->unk18 >= 0x10) {
        show = FALSE;
    }
    if (show) {
        spr = &w->spr[2];
        spr->x = x + 0x20;
        ys = y + 0x50;
        spr->y = ys;
        a = 0x30;
        if (p->unk30 == 0) {
            if (p->unk34 == 0) a++;
            if (p->unk34 == p->unk30) {
                if (p->unk18 < 8) {
                    if (p->unk2c == 0) m = 3; else m = 2;
                } else {
                    m = 0;
                }
            } else {
                if (p->unk14 < 7) m = 1; else m = 0;
            }
            func_ov001_022ca248(p, spr->x, spr->y, m);
        }
        if (w->anim[2] != a) {
            func_02033550(spr, a);
            w->anim[2] = a;
        }
        spr->unk46 = 4;
        func_0203344c(spr, 0, 0);
        func_02032d2c(spr);

        spr = &w->spr[3];
        x += 0x88;
        spr->x = x;
        spr->y = ys;
        a = 0x32;
        if (p->unk34 != 0) a++;
        if (p->unk30 == 1) {
            if (p->unk34 == p->unk30) {
                if (p->unk18 < 8) m = 2; else m = 0;
            } else {
                if (p->unk14 < 7) m = 1; else m = 0;
            }
            func_ov001_022ca248(p, spr->x, spr->y, m);
        }
        if (w->anim[3] != a) {
            func_02033550(spr, a);
            w->anim[3] = a;
        }
        spr->unk46 = 4;
        func_0203344c(spr, 0, 0);
        func_02032d2c(spr);
    }

    spr = &w->spr[4];
    if (w->anim[4] != 0x34) {
        func_02033550(spr, 0x34);
        w->anim[4] = 0x34;
    }
    spr->x = xs;
    y += 0x98;
    spr->y = y;
    func_0203344c(spr, 0, 0);
    func_02032d2c(spr);
}
#pragma thumb reset
