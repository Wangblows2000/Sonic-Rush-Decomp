#include <nitro/types.h>


u16 ObjDraw_ShiftColor(u16 c, s32 dr, s32 dg, s32 db) {
    s16 r = (c & 0x1f) + dr;
    s16 g = ((c & 0x3e0) >> 5) + dg;
    s16 b = ((c & 0x7c00) >> 10) + db;

    if (r > 0x1f)
        r = 0x1f;
    else if (r < 0)
        r = 0;
    if (g > 0x1f)
        g = 0x1f;
    else if (g < 0)
        g = 0;
    if (b > 0x1f)
        b = 0x1f;
    else if (b < 0)
        b = 0;
    return r | (g << 5) | (b << 10);
}
