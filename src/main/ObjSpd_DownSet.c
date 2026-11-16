#include <nitro/types.h>

extern s16 data_0207cf7c;

/* Moves a signed value towards zero by (b * data_0207cf7c) >> 8, clamping at 0.
 * Matched (CW 1.2). */
s32 ObjSpd_DownSet(s32 a, s32 b) {
    if (a > 0) {
        a -= (b * data_0207cf7c) >> 8;
        if (a < 0) a = 0;
    } else {
        a += (b * data_0207cf7c) >> 8;
        if (a > 0) a = 0;
    }
    return a;
}
