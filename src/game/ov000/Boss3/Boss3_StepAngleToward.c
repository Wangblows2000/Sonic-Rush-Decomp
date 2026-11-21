#include <nitro/types.h>

u16 Boss3_StepAngleToward(u16 cur, u16 target, u16 step) {
    u16 d;
    if (target == cur)
        return cur;
    d = target - cur;
    if (d < 0x8000) {
        if (d < step)
            step = d;
        return cur + step;
    }
    d = cur - target;
    if (d < step)
        step = d;
    return cur - step;
}
