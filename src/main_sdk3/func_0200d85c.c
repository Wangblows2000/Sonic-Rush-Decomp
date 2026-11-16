#include <nitro/types.h>

typedef struct {
    s16 x0;
    s16 y0;
    s16 xDotSize;
    s16 yDotSize;
} UnkTPCalib_0200d85c;

typedef struct {
    u8 pad0[0x58];
    u16 rawX1;   /* 0x58 */
    u16 rawY1;   /* 0x5a */
    u8 dx1;      /* 0x5c */
    u8 dy1;      /* 0x5d */
    u16 rawX2;   /* 0x5e */
    u16 rawY2;   /* 0x60 */
    u8 dx2;      /* 0x62 */
    u8 dy2;      /* 0x63 */
} UnkNvram_0200d85c;

BOOL func_0200d2d4(UnkTPCalib_0200d85c *calib, u16 rawX1, u16 rawY1, u16 dx1, u16 dy1, u16 rawX2, u16 rawY2, u16 dx2, u16 dy2);

BOOL func_0200d85c(UnkTPCalib_0200d85c *calib)
{
    const UnkNvram_0200d85c *nv = (const UnkNvram_0200d85c *)0x027ffc80;
    u16 x1;
    u16 y1;
    u8 dx1;
    u16 x2;
    u16 y2;
    u8 dy1;
    u8 dx2;
    u8 dy2;
    x1 = nv->rawX1;
    y1 = nv->rawY1;
    dx1 = nv->dx1;
    dy1 = nv->dy1;
    x2 = nv->rawX2;
    y2 = nv->rawY2;
    dx2 = nv->dx2;
    dy2 = nv->dy2;

    if ((x1 == 0 && x2 == 0 && y1 == 0 && y2 == 0) || func_0200d2d4(calib, x1, y1, dx1, dy1, x2, y2, dx2, dy2)) {
        calib->x0 = 0;
        calib->y0 = 0;
        calib->xDotSize = 0;
        calib->yDotSize = 0;
        return TRUE;
    }
    return TRUE;
}
