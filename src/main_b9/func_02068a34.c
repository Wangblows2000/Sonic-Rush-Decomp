#include <nitro/types.h>

u16 func_02068a34(u16 cur, u16 tgt, u16 step) {
    u16 d1;
    u16 d2;

    if (tgt == cur)
        return tgt;
    d1 = cur - tgt < 0 ? -(cur - tgt) : cur - tgt;
    if (cur > tgt)
        d2 = 0x10000 - cur + tgt;
    else
        d2 = 0x10000 - tgt + cur;
    if (d1 <= d2) {
        if (cur > tgt) {
            if (tgt > cur - step)
                cur = tgt;
            else
                cur -= step;
        } else if (cur < tgt) {
            if (tgt < cur + step)
                cur = tgt;
            else
                cur += step;
        }
    } else {
        if (cur > tgt) {
            if (tgt + 0x10000 < cur + step)
                cur = tgt;
            else
                cur += step;
        } else if (cur < tgt) {
            if (tgt > cur - step + 0x10000)
                cur = tgt;
            else
                cur -= step;
        }
    }
    return cur;
}
