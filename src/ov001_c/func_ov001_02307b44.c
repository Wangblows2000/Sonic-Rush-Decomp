#include <nitro/types.h>

typedef struct UnkStruct_02307b44_o {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkStruct_02307b44_o;

typedef struct UnkStruct_02307b44 {
    u8 pad0[0x108];
    UnkStruct_02307b44_o objs[0x17];
    u8 pad780[0xbd4 - 0x780];
    s32 timer;
    u8 padBD8[0xbf8 - 0xbd8];
    s16 unkBF8;
    s16 unkBFA;
    UnkStruct_02307b44_o objs2[3];
} UnkStruct_02307b44;

extern UnkStruct_02307b44 *data_ov001_0231f5bc;

void func_02032d2c(void *p);
u32 func_ov001_0230c0b8(void);
s32 func_ov001_0230c0cc(void);
void func_02032480(void *o, s32 a, s32 b, s32 c);

void func_ov001_02307b44(void) {
    UnkStruct_02307b44 *g = data_ov001_0231f5bc;
    UnkStruct_02307b44_o *a = &g->objs[0x15];
    UnkStruct_02307b44_o *b = &g->objs[0x16];
    UnkStruct_02307b44_o *o19;
    UnkStruct_02307b44_o *o;
    s32 i;
    s32 d;
    s32 s;
    s16 sx;
    s16 sy;

    if (g->timer <= 0x3c)
        func_02032d2c(a);
    else
        func_02032d2c(b);
    for (i = 0; i < 7; i++) {
        u32 m = func_ov001_0230c0b8();
        if ((m & (1 << i)) && i != func_ov001_0230c0cc()) {
            func_02032d2c(&g->objs[0xb + i]);
        } else {
            g->objs[10].x = g->objs[0xb + i].x;
            g->objs[10].y = g->objs[0xb + i].y;
            func_02032d2c(&g->objs[10]);
        }
    }
    if (g->timer < 0xf0)
        return;
    o19 = &g->objs[19];
    o = &g->objs[func_ov001_0230c0cc() + 0xb];
    d = g->timer - 0xf0;
    if (g->timer < 0xf5) {
        s = (d << 12) / 7 + 0x800;
        if (s > 0x1800)
            s = 0x1800;
    } else {
        s = 0x1800 - ((g->timer - 0xf5) * 0x1800) / 7;
        if (s < 0)
            s = 0;
    }
    if (s > 0) {
        o19->x = 0x80;
        o19->y = 0x78;
        func_02032480(o19, s, s, 0);
    }
    if (g->timer < 0xf5)
        return;
    sx = o->x;
    sy = o->y;
    o->x = g->unkBF8;
    o->y = g->unkBFA;
    func_02032d2c(o);
    o->x = sx;
    o->y = sy;
    for (i = 0; i < 3; i++)
        func_02032d2c(&g->objs2[i]);
}
