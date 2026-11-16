#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x2c - 8];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022d39ac;

typedef struct {
    u32 timer;
    s32 sel;
    u8 pad8[4];
    UnkSpr_022d39ac spr[6];
    s32 anim[6];
    u8 pad1d4[0x5f4 - 0x1d4];
    u32 vals[4];
    void (*fns[4])(void);
} UnkWork_022d39ac;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022d39ac *work;
} UnkTask_022d39ac;

extern UnkTask_022d39ac *data_022b4574;

void func_02033550(UnkSpr_022d39ac *s, u16 id);
void func_0203344c(UnkSpr_022d39ac *s, s32 a, s32 b);
void func_02032d2c(UnkSpr_022d39ac *s);
void func_ov001_022d3538(s32 a, s32 b, s32 c, s32 d);
void func_ov001_022d3864(void);

#pragma thumb on
void func_ov001_022d39ac(void) {
    UnkWork_022d39ac *w;
    UnkSpr_022d39ac *s;
    s32 i;
    u16 id;

    w = data_022b4574->work;
    s = &w->spr[0];
    if (w->anim[0] != 0) {
        func_02033550(s, 0);
        w->anim[0] = 0;
    }
    s->flags &= ~4;
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
    if (w->timer >= 0x20) {
        w->timer = 0;
        func_ov001_022d3538(0, 0x10, 0x10, 0);
        data_022b4574->func = func_ov001_022d3864;
    }
}
#pragma thumb reset
