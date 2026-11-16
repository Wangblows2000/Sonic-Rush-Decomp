#include <nitro/types.h>

s32 func_02068bd8(s32 cur, s32 target, s32 ref, s32 shift, u16 maxStep, s32 minStep) {
    s32 d;
    s32 diff;

    if (cur == target)
        return cur;
    if (minStep == 0)
        minStep = 1;
    d = (cur - ref) >> shift;
    if (target > ref && d < 0)
        d = 0;
    if (target < ref && d > 0)
        d = 0;
    if (maxStep != 0) {
        if (d > maxStep)
            d = maxStep;
        if (d < -maxStep)
            d = -maxStep;
    }
    if (minStep != 0) {
        if (d > 0) {
            if (d < minStep)
                d = minStep;
        } else if (d < 0) {
            if (d > -minStep)
                d = -minStep;
        } else {
            diff = target - cur;
            if (diff > 0 && d < minStep)
                d = minStep;
            if (diff < 0 && d > -minStep)
                d = -minStep;
        }
    }
    cur += d;
    if (d > 0) {
        if (cur > target)
            cur = target;
    } else if (d < 0) {
        if (cur < target)
            cur = target;
    }
    return cur;
}
