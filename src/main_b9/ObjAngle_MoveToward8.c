#include <nitro/types.h>

u8 ObjAngle_MoveToward8(u8 cur, u8 tgt, u8 step) {
    u8 d1;
    u8 d2;

    if (tgt == cur)
        return tgt;
    d1 = cur - tgt < 0 ? -(cur - tgt) : cur - tgt;
    if (cur > tgt)
        d2 = 0x100 - cur + tgt;
    else
        d2 = 0x100 - tgt + cur;
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
            if (tgt + 0x100 < cur + step)
                cur = tgt;
            else
                cur += step;
        } else if (cur < tgt) {
            if (tgt > cur - step + 0x100)
                cur = tgt;
            else
                cur -= step;
        }
    }
    return cur;
}
