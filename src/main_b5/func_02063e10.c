#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x40];
} UnkSpr_02063e10;

typedef struct {
    u8 pad0[0x10];
    UnkSpr_02063e10 spr[25];
    s16 sel0;
    s16 sel1;
    s32 unk71c;
} UnkWork_02063e10;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02063e10 *work;
} UnkTask_02063e10;

extern UnkTask_02063e10 *data_022b4574;

void func_02032d2c(UnkSpr_02063e10 *s);

void func_02063e10(void) {
    UnkWork_02063e10 *w = data_022b4574->work;
    UnkSpr_02063e10 *s;

    func_02032d2c(&w->spr[13]);
    w->spr[10].x = -2;
    func_02032d2c(&w->spr[10]);
    if (w->sel0 != -1) {
        s = &w->spr[w->sel0];
        s->x = 0x39;
        s->y = 0x57;
        func_02032d2c(s);
    }
    s = &w->spr[w->sel1];
    s->x = 0x43;
    s->y = 0x57;
    func_02032d2c(s);
    if (w->unk71c != 0)
        func_02032d2c(&w->spr[24]);
}
