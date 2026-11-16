#include <nitro/types.h>

s32 ObjShift_Set(s32 cur, s32 target, s32 shift, s32 maxStep, s32 minStep) {
    s32 d;
    s32 diff;

    if (cur == target)
        return cur;
    diff = target - cur;
    if (minStep == 0)
        minStep = 1;
    d = diff >> shift;
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
