#include <nitro/types.h>

#define CLAMP(v, lo, hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))

void ZoneFx_Zone2_TintPalette(void *unused, u16 *pal) {
    u16 i;
    u16 col;
    s8 r, g, b;
    s32 v;

    i = 0;
    do {
        col = pal[i];
        r = col & 0x1f;
        g = (col & 0x3e0) >> 5;
        b = (col & 0x7c00) >> 10;
        v = r - 10;
        r = CLAMP(v, 0, 0x1f);
        v = g - 1;
        g = CLAMP(v, 0, 0x1f);
        v = b;
        b = CLAMP(v, 0, 0x1f);
        pal[i] = r | (g << 5) | (b << 10);
        i++;
    } while (i < 0x100);
}
