#include <nitro/types.h>

void func_ov000_0234c644(u8 *w);

#define ABS(x) ((x) < 0 ? -(x) : (x))

BOOL func_ov000_023470ec(u8 *w) {
    s32 v;

    if (!(*(u32 *)(w + 0x58) & 1)) {
        func_ov000_0234c644(w);
        return TRUE;
    }
    if (w[0x3cc] != 0) {
        w[0x3cc]--;
    } else {
        if (!((w[1] + 0x40) & 0x80) && w[1] != 0xc0)
            return FALSE;
        v = *(s16 *)(w + 0x34);
        if (ABS(v) >= 0x1e0)
            return FALSE;
        func_ov000_0234c644(w);
        return TRUE;
    }
    return FALSE;
}
