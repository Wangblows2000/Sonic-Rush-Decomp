#include <nitro/types.h>

typedef struct {
    u32 timer;
    u8 pad4[0xd8 - 4];
    s32 sub;
} UnkWork_022e114c;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022e114c *work;
} UnkTask_022e114c;

typedef struct {
    u8 pad0[8];
    s16 bright;
} UnkBright_022e114c;

extern UnkTask_022e114c *data_022b4574;
extern UnkBright_022e114c data_0208926c;
extern UnkBright_022e114c data_02087d44;
extern s32 data_ov001_0231c3ec;
extern s32 data_ov001_0231c3f0;
void func_020371c8(UnkTask_022e114c *t);

s32 func_0200225c(s32 n, s32 d);
void GXx_SetMasterBrightness_(vu16 *reg, s32 v);
void func_ov001_022e08e0(UnkWork_022e114c *w);
void func_ov001_022e09d0(UnkWork_022e114c *w);
void func_ov001_022e0ad0(UnkWork_022e114c *w);
void func_ov001_022e0c5c(UnkWork_022e114c *w);
void func_ov001_022e0dfc(UnkWork_022e114c *w);
void func_ov001_022e0d30(UnkWork_022e114c *w);
void func_ov001_022e0884(UnkWork_022e114c *w);
void func_ov001_022e0858(UnkWork_022e114c *w, s32 a);
void func_02064674(void);
void func_02064788(void);
void func_ov001_022e0f94(void);

#pragma thumb on
static inline fx32 FX_Mul_022e114c(fx32 a, fx32 b) {
    return (fx32)(((s64)a * b + 0x800) >> 12);
}

void func_ov001_022e114c(void) {
    UnkWork_022e114c *w;
    s32 target;
    fx32 a;
    fx32 b;
    s32 v;

    w = data_022b4574->work;
    if (data_ov001_0231c3ec < 0 || data_ov001_0231c3f0 < 0) {
        target = 0x10;
    } else if (data_ov001_0231c3f0 == 2) {
        target = -0x10;
    } else {
        target = 0x10;
    }
    a = 0 << 12;
    b = target << 12;
    if (a >= 0) a += 0x800; else a -= 0x800;
    if (b >= 0) b += 0x800; else b -= 0x800;
    v = (a + func_0200225c(FX_Mul_022e114c(w->timer << 12, b - a), 30 << 12)) >> 12;
    if (v < -0x10) v = -0x10;
    if (v > 0x10) v = 0x10;
    data_0208926c.bright = v;
    if (w->sub != 0) {
        data_02087d44.bright = v;
    }
    w->timer++;
    if (w->timer < 0x1e) {
        func_ov001_022e08e0(w);
        func_ov001_022e09d0(w);
        func_ov001_022e0ad0(w);
        func_ov001_022e0c5c(w);
        func_ov001_022e0dfc(w);
        func_ov001_022e0d30(w);
        func_ov001_022e0884(w);
        func_ov001_022e0858(w, data_ov001_0231c3ec);
    }
    if (w->timer >= 0x1e) {
        w->timer = 0;
        if (w->sub != 0) {
            data_0208926c.bright = (s16)target;
            data_02087d44.bright = (s16)target;
            GXx_SetMasterBrightness_((vu16 *)0x400006c, data_0208926c.bright);
            GXx_SetMasterBrightness_((vu16 *)0x400106c, data_02087d44.bright);
        } else {
            data_0208926c.bright = target;
            GXx_SetMasterBrightness_((vu16 *)0x400006c, data_0208926c.bright);
            data_ov001_0231c3ec = -1;
            data_ov001_0231c3f0 = -1;
        }
        func_020371c8(data_022b4574);
    }
}
#pragma thumb reset
