#include <nitro/types.h>

extern u8 data_ov000_02364db0[];

typedef struct {
    u16 col[256];
    u8 add[2];
} UnkPal_0233c5d0;

#define CLAMP(v, lo, hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))

void func_ov000_0233c5d0(UnkPal_0233c5d0 *pal, s32 which) {
    u8 *tbl = data_ov000_02364db0;
    u8 k = 0;
    s32 v;
    u16 col;
    s8 r, g, b;
    u16 *dst;
    u16 i;
    u8 add;

    if (which == 0)
        dst = (u16 *)0x5000000;
    else
        dst = (u16 *)0x5000400;
    add = pal->add[which];
    i = 0;
    do {
        if (!(k & 1)) {
            i = tbl[k++];
        } else if (i == tbl[k]) {
            k++;
        }
        col = pal->col[i];
        r = col & 0x1f;
        g = (col & 0x3e0) >> 5;
        b = (col & 0x7c00) >> 10;
        v = r + add;
        r = CLAMP(v, 0, 0x1f);
        dst[i] = r | (g << 5) | (b << 10);
        i++;
    } while (i < 0x100);
}
