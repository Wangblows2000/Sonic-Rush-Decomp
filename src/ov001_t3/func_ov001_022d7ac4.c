#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x2c - 8];
    u32 flags;
    u8 pad30[0x48 - 0x30];
} UnkSpr_022d7ac4;

typedef struct {
    u8 pad0[0x25c];
    UnkSpr_022d7ac4 spr[9];
    u8 pad[0x7b4 - (0x25c + 9 * 0x48)];
    s32 anim[9];
} UnkObj_022d7ac4;

typedef struct {
    u8 pad0[0x1a4];
    UnkObj_022d7ac4 *obj;
} UnkWork_022d7ac4;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d7ac4 *work;
} UnkTask_022d7ac4;

extern UnkTask_022d7ac4 *data_022b4574;

void func_02033550(UnkSpr_022d7ac4 *s, u16 id);
void func_0203344c(UnkSpr_022d7ac4 *s, s32 a, s32 b);
void func_02032d2c(UnkSpr_022d7ac4 *s);

#pragma thumb on
void func_ov001_022d7ac4(s32 idx, s16 x, s16 y) {
    UnkWork_022d7ac4 *w;
    UnkSpr_022d7ac4 *s;
    s32 anim;

    w = data_022b4574->work;
    switch (idx) {
    case 2:
        anim = 0;
        break;
    case 3:
        anim = 1;
        break;
    case 4:
        anim = 2;
        break;
    case 5:
        anim = 3;
        break;
    case 6:
        anim = 4;
        break;
    case 7:
        anim = 5;
        break;
    case 8:
        anim = 6;
        break;
    }
    s = &w->obj->spr[idx];
    if (anim != w->obj->anim[idx]) {
        func_02033550(s, anim);
        w->obj->anim[idx] = anim;
    }
    s->x = x;
    s->y = y;
    s->flags &= ~4;
    s->flags &= ~1;
    func_0203344c(s, 0, 0);
    func_02032d2c(s);
}
#pragma thumb reset
