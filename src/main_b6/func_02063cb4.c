#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x40];
} UnkSpr_02063cb4;

typedef struct {
    u8 pad0[0x10];
    UnkSpr_02063cb4 spr[1];
} UnkWork_02063cb4;

typedef struct {
    u8 *w;
    u32 pad4;
    s16 a;
    s16 b;
} UnkCur_02063cb4;

void func_02032d2c(void *s);

void func_02063cb4(UnkCur_02063cb4 *c) {
    u8 *w = c->w;

    func_02032d2c(w + 0x370);
    func_02032d2c(w + 0x328);
    if (c->a != -1) {
        UnkSpr_02063cb4 *s = &((UnkWork_02063cb4 *)w)->spr[c->a];
        s->x = 0x2f;
        s->y = 0x6f;
        func_02032d2c(s);
    }
    {
        UnkSpr_02063cb4 *s = &((UnkWork_02063cb4 *)w)->spr[c->b];
        s->x = 0x39;
        s->y = 0x6f;
        func_02032d2c(s);
    }
}
