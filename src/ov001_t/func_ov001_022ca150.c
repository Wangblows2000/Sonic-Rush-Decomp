#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x3e];
    u16 unk46;
} UnkSpr_022ca150;

typedef struct {
    u8 pad0[0x584];
    UnkSpr_022ca150 spr[8];
    s32 anim[8];
} UnkWork_022ca150;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[0x3ac - 0x14];
    UnkWork_022ca150 *work;
} UnkStruct_022ca150;

void func_02033550(UnkSpr_022ca150 *s, u16 anim);
void func_0203344c(UnkSpr_022ca150 *s, u32 a, u32 b);
void func_02032d2c(UnkSpr_022ca150 *s);

#pragma thumb on
void func_ov001_022ca150(UnkStruct_022ca150 *p, s16 x, s16 y, s32 mode) {
    UnkWork_022ca150 *w;
    UnkSpr_022ca150 *spr;
    u16 a;

    if (p->unk10 == 0) return;
    w = p->work;
    spr = &w->spr[2];
    a = 3;
    switch (mode) {
    case 0:
        break;
    case 1:
        a += 1;
        break;
    case 2:
        a += 2;
        break;
    }
    if (w->anim[2] != a) {
        func_02033550(spr, a);
        w->anim[2] = a;
    }
    spr->x = x;
    spr->y = y;
    spr->unk46 = 1;
    func_0203344c(spr, 0, 0);
    func_02032d2c(spr);
}
#pragma thumb reset
