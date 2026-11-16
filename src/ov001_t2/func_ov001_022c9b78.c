#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x24];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022c9b78;


typedef struct {
    u8 pad0[0xafc];
    UnkSpr_022c9b78 spr0;
    UnkSpr_022c9b78 spr1;
    u8 padb8c[0xc00 - 0xb8c];
    s32 anim0;
    s32 anim1;
} UnkWork_022c9b78;

typedef struct {
    u8 pad0[0x3ac];
    UnkWork_022c9b78 *work;
} UnkStruct_022c9b78;

void func_02033550(UnkSpr_022c9b78 *s, u16 anim);
void func_0203344c(UnkSpr_022c9b78 *s, u32 a, u32 b);
void func_02032d2c(UnkSpr_022c9b78 *s);

#pragma thumb on
void func_ov001_022c9b78(UnkStruct_022c9b78 *p, s16 x, s16 y) {
    UnkWork_022c9b78 *w = p->work;
    UnkSpr_022c9b78 *s = &w->spr0;

    if (w->anim0 != 0x14) {
        func_02033550(s, 0x14);
        w->anim0 = 0x14;
    }
    s->x = x + 0x4c;
    s->y = y + 0x3c;
    func_0203344c(s, 0, 0);
    func_02032d2c(s);
    s = &w->spr1;
    if (w->anim1 != 0x14) {
        func_02033550(s, 0x14);
        w->anim1 = 0x14;
    }
    s->x = x + 0xb4;
    s->y = y + 0x3c;
    s->flags |= 0x80;
    func_0203344c(s, 0, 0);
    func_02032d2c(s);
}
#pragma thumb reset
