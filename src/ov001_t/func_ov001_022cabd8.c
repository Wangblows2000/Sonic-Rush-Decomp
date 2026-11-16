#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x3e];
    u16 unk46;
} UnkSpr_022ca9f4;

typedef struct {
    u32 unk0;
    s32 unk4;
} UnkCfg_022ca9f4;

typedef struct {
    u8 pad0[8];
    UnkCfg_022ca9f4 cfg;
    u8 pad10[0x408 - 0x10];
    UnkSpr_022ca9f4 spr[5];
    s32 anim[5];
} UnkWork_022ca9f4;

typedef struct {
    u8 pad0[0x14];
    u32 unk14;
    s32 unk18;
    s32 unk1c;
    u8 pad20[0x30 - 0x20];
    s32 unk30;
    s32 unk34;
    u8 pad38[0x3ac - 0x38];
    UnkWork_022ca9f4 *work;
} UnkStruct_022ca9f4;

void func_02033550(UnkSpr_022ca9f4 *s, u16 anim);
void func_0203344c(UnkSpr_022ca9f4 *s, u32 a, u32 b);
void func_02032d2c(UnkSpr_022ca9f4 *s);
void func_ov001_022ca2d4(UnkStruct_022ca9f4 *p, s32 x, s32 y, s32 a);
void func_ov001_022ca248(UnkStruct_022ca9f4 *p, s32 x, s32 y, s32 m);

#pragma thumb on
void func_ov001_022cabd8(UnkStruct_022ca9f4 *p, s32 x, s32 y) {
    UnkCfg_022ca9f4 *cfg;
    s32 ys;
    s32 xs;
    UnkWork_022ca9f4 *w;
    UnkSpr_022ca9f4 *spr;
    u16 a;
    s32 m;

    if (x >= 0x100) return;
    w = p->work;
    cfg = &w->cfg;
    spr = &w->spr[0];
    if (w->anim[0] != 0x16) {
        func_02033550(spr, 0x16);
        w->anim[0] = 0x16;
    }
    xs = x + 0x18;
    spr->x = xs;
    spr->y = y + 0x18;
    func_0203344c(spr, 0, 0);
    func_02032d2c(spr);
    if (p->unk1c >= 0) {
        func_ov001_022ca2d4(p, x, y, 1);
    } else {
        func_ov001_022ca2d4(p, x, y, 0);
    }

    spr = &w->spr[2];
    spr->x = x + 0x20;
    ys = y + 0x50;
    spr->y = ys;
    a = 0x28;
    if (cfg->unk0 == 0) a++;
    if (p->unk30 == 0) {
        if (p->unk34 == p->unk30) {
            if (p->unk18 < 8) m = 2; else m = 0;
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
    a = 0x2a;
    if (cfg->unk0 == 1) a++;
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
