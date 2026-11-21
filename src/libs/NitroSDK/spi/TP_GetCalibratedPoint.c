#include <nitro/types.h>

typedef struct {
    u16 x;
    u16 y;
    u16 touch;
    u16 validity;
} UnkTPData_0200d1a4;

typedef struct {
    s32 x0;
    s32 w;
    s32 xDotSize;
    s32 y0;
    s32 h;
    s32 yDotSize;
} UnkTPCalib_0200d1a4;

typedef struct {
    u8 pad0[0x30];
    u16 enable;   /* 0x30 */
} UnkWork_0200d1a4;

extern UnkWork_0200d1a4 data_0207f02c;
extern UnkTPCalib_0200d1a4 data_0207f044;

void TP_GetCalibratedPoint(UnkTPData_0200d1a4 *disp, const UnkTPData_0200d1a4 *raw)
{
    UnkTPCalib_0200d1a4 *c;
    if (data_0207f02c.enable == 0) {
        *disp = *raw;
        return;
    }
    c = &data_0207f044;
    disp->touch = raw->touch;
    disp->validity = raw->validity;
    if (raw->touch == 0) {
        disp->x = 0;
        disp->y = 0;
        return;
    }
    disp->x = (u16)((((s64)(raw->x * 4) - c->x0) * c->xDotSize) >> 22);
    if ((s16)disp->x < 0) {
        disp->x = 0;
    } else if ((s16)disp->x > 0xff) {
        disp->x = 0xff;
    }
    disp->y = (u16)((((s64)(raw->y * 4) - c->y0) * c->yDotSize) >> 22);
    if ((s16)disp->y < 0) {
        disp->y = 0;
    } else if ((s16)disp->y > 0xbf) {
        disp->y = 0xbf;
    }
}
