#include <nitro/types.h>

s32 func_020660f4(s32 y, s32 x, u32 flags, u8 *angleOut, u8 *attrOut);
s32 func_02066360(s32 x, s32 y, u32 flags, u8 *angleOut, u8 *attrOut);

s32 MapCollision_ProbeDistance(s32 x, s32 y, u32 flags, u32 mode, u8 *angleOut, u8 *attrOut) {
    u8 angle;
    u8 attr;
    s32 dir = 8;
    s32 h;
    s8 ofs;

    if (angleOut != NULL)
        angle = *angleOut;
    if (attrOut != NULL)
        attr = *attrOut;
    if (mode & 1)
        dir = -8;
    if (mode & 2) {
        h = func_020660f4(y, x, flags, angleOut, attrOut);
        ofs = y & 7;
    } else {
        h = func_02066360(x, y, flags, angleOut, attrOut);
        ofs = x & 7;
    }
    if (h == 0) {
        if (angleOut != NULL)
            *angleOut = angle;
        if (attrOut != NULL)
            *attrOut = attr;
        if (dir > 0)
            return 8 - ofs;
        return ofs + 1;
    }
    if (h == 8) {
        if (dir > 0)
            return -(ofs + 1);
        return ofs - 8;
    }
    if (h > 0) {
        if (dir > 0)
            return h - (ofs + 1);
        return 8 - ofs;
    }
    if (dir > 0)
        return -(ofs + 1);
    return h + ofs;
}
