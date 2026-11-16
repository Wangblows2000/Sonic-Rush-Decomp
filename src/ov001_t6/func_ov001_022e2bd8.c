#include <nitro/types.h>

typedef struct {
    s32 m[16];
} UnkMtx44_022e2bd8;

void func_0200468c(s32 t, s32 b, s32 l, s32 r, s32 n, s32 f, s32 scaleW, BOOL draw, UnkMtx44_022e2bd8 *mtx);
void func_02025268(u32 op, const void *args, u32 n);
void func_ov001_022e2bbc(void);
void func_ov001_022e2ba4(BOOL a);

#pragma thumb on
void func_ov001_022e2bd8(s32 a, s32 b, s32 c, s32 d, u16 color) {
    s16 x1;
    s16 dx;
    s32 step;
    s32 n;
    s16 x0;
    u32 mode0;
    u32 mode1;
    u32 col;
    u32 attr;
    u32 prim;
    u32 v0[2];
    u32 v1[2];
    u32 v2[2];
    u32 v3[2];
    u32 v4[2];
    u32 v5[2];
    UnkMtx44_022e2bd8 mtx;

    if (c >= d) {
        n = c - d + 1;
        step = -1;
    } else {
        n = d - c + 1;
        step = 1;
    }
    x0 = (a << 12) / 0x100;
    x1 = (b << 12) / 0x100;
    dx = (x1 - x0) / n;
    func_0200468c(0, 0xc00, 0, 0x1000, -0x1000, 0x1000, 0x1000, 0, &mtx);
    mode0 = 0;
    func_02025268(0x10, &mode0, 1);
    func_02025268(0x15, NULL, 0);
    func_02025268(0x16, &mtx, 0x10);
    mode1 = 1;
    func_02025268(0x10, &mode1, 1);
    func_02025268(0x15, NULL, 0);
    func_ov001_022e2bbc();
    *(vu16 *)0x4000060 = (*(vu16 *)0x4000060 & ~0x3000) | 8;
    *(vu16 *)0x4000060 &= 0xcffb;
    col = color;
    func_02025268(0x20, &col, 1);
    func_ov001_022e2ba4(1);
    for (; c != d + step; c += step) {
        if (c > 0) {
            attr = (c << 16) | 0xc0;
            func_02025268(0x29, &attr, 1);
            prim = 1;
            func_02025268(0x40, &prim, 1);
            v0[0] = ((u16)x0 << 16) | 0x1000;
            v0[1] = 0xf001;
            func_02025268(0x23, v0, 2);
            v1[0] = (u16)x0 << 16;
            v1[1] = 0xf001;
            func_02025268(0x23, v1, 2);
            if (c == d) {
                v2[0] = (u16)x1 << 16;
                v2[1] = 0xf001;
                func_02025268(0x23, v2, 2);
                v3[0] = ((u16)x1 << 16) | 0x1000;
                v3[1] = 0xf001;
                func_02025268(0x23, v3, 2);
            } else {
                v4[0] = (u16)(s16)(x0 + dx) << 16;
                v4[1] = 0xf001;
                func_02025268(0x23, v4, 2);
                v5[0] = ((u16)(s16)(x0 + dx) << 16) | 0x1000;
                v5[1] = 0xf001;
                func_02025268(0x23, v5, 2);
            }
            func_02025268(0x41, NULL, 0);
        }
        x0 += dx;
    }
}
#pragma thumb reset
