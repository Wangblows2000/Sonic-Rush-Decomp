#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x2c - 8];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022d3864;

typedef struct {
    u32 timer;
    s32 sel;
    u8 pad8[4];
    UnkSpr_022d3864 spr[6];
    s32 anim[6];
    u8 pad1d4[0x5f4 - 0x1d4];
    u32 vals[4];
    void (*fns[4])(void);
} UnkWork_022d3864;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d3864 *work;
} UnkTask_022d3864;

extern UnkTask_022d3864 *data_022b4574;

void func_02033550(UnkSpr_022d3864 *s, u16 id);
void func_0203344c(UnkSpr_022d3864 *s, s32 a, s32 b);
void func_02032d2c(UnkSpr_022d3864 *s);
void func_02036fdc(s32 a);

#pragma thumb on
void func_ov001_022d3864(void) {
    UnkWork_022d3864 *w;
    UnkSpr_022d3864 *s;
    s32 i;
    u16 id;
    void (*fn)(void);

    w = data_022b4574->work;
    s = &w->spr[0];
    if (w->anim[0] != 0) {
        func_02033550(s, 0);
        w->anim[0] = 0;
    }
    s->flags &= ~4;
    s->x = 0;
    s->y = 0;
    func_0203344c(s, 0, 0);
    func_02032d2c(s);
    for (i = 0; i < 4; i++) {
        if (w->sel == i) {
            s = &w->spr[1];
            if (w->anim[1] != 0) {
                func_02033550(s, 0);
                w->anim[1] = 0;
            }
            s->flags &= ~4;
            s->x = 0x40;
            s->y = i * 0x20 + 0x24;
            func_0203344c(s, 0, 0);
            func_02032d2c(s);
        }
        s = &w->spr[2 + i];
        id = w->vals[i];
        if (w->sel == i) {
            id += 2;
            s->flags |= 4;
        } else {
            s->flags &= ~4;
        }
        if (id != w->anim[2 + i]) {
            func_02033550(s, id);
            w->anim[2 + i] = id;
        }
        func_0203344c(s, 0, 0);
        func_02032d2c(s);
    }
    w->timer++;
    if (w->timer >= 0x10) {
        fn = w->fns[w->sel];
        func_02036fdc(0x1000);
        fn();
    }
}
#pragma thumb reset
