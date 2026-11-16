#include <nitro/types.h>

typedef struct {
    s16 x0;
    s16 y0;
    s16 xDotSize;
    s16 yDotSize;
} UnkTPCalib_0200d2d4;

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);

#define REG_DIVCNT (*(vu16 *)0x04000280)
#define REG_DIV_NUMER (*(vu32 *)0x04000290)
#define REG_DIV_DENOM (*(volatile u64 *)0x04000298)
#define REG_DIV_RESULT (*(vs32 *)0x040002a0)

static inline void func_0200d2d4_SetDiv(u32 numer, u32 denom)
{
    REG_DIVCNT = 0;
    REG_DIV_NUMER = numer;
    REG_DIV_DENOM = denom;
}

static inline void func_0200d2d4_Wait(void)
{
    while (REG_DIVCNT & 0x8000) {
    }
}

BOOL func_0200d2d4(UnkTPCalib_0200d2d4 *calib, u16 rawX1, u16 rawY1, u16 dx1, u16 dy1, u16 rawX2, u16 rawY2, u16 dx2, u16 dy2)
{
    u32 enabled;
    s32 rx;
    s32 ry;
    s32 x;
    s32 yd;
    s32 dyd;

    if (rawX1 >= 0x1000 || rawY1 >= 0x1000 || rawX2 >= 0x1000 || rawY2 >= 0x1000) {
        return TRUE;
    }
    if (dx1 >= 0x100 || dx2 >= 0x100 || dy1 >= 0xc0 || dy2 >= 0xc0) {
        return TRUE;
    }
    if (dx1 == dx2 || dy1 == dy2 || rawX1 == rawX2 || rawY1 == rawY2) {
        return TRUE;
    }
    enabled = OS_DisableInterrupts();
    func_0200d2d4_SetDiv((rawX1 - rawX2) << 8, dx1 - dx2);
    yd = rawY1 - rawY2;
    dyd = dy1 - dy2;
    func_0200d2d4_Wait();
    rx = REG_DIV_RESULT;
    func_0200d2d4_SetDiv(yd << 8, dyd);
    if (rx >= 0x8000 || rx < -0x8000) {
        OS_RestoreInterrupts(enabled);
        return TRUE;
    }
    calib->xDotSize = rx;
    x = (s16)((((rawX1 + rawX2) << 8) - calib->xDotSize * (dx1 + dx2)) >> 7);
    if (x >= 0x8000 || x < -0x8000) {
        OS_RestoreInterrupts(enabled);
        return TRUE;
    }
    calib->x0 = x;
    func_0200d2d4_Wait();
    ry = REG_DIV_RESULT;
    OS_RestoreInterrupts(enabled);
    if (ry >= 0x8000 || ry < -0x8000) {
        return TRUE;
    }
    calib->yDotSize = ry;
    x = (s16)((((rawY1 + rawY2) << 8) - calib->yDotSize * (dy1 + dy2)) >> 7);
    if (x >= 0x8000 || x < -0x8000) {
        return TRUE;
    }
    calib->y0 = x;
    return FALSE;
}
