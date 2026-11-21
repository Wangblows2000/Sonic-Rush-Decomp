#include <nitro/types.h>

s32 Boss7_UpdateMashPower(s32 a, s32 b) {
    if (a & 3)
        return b + (((0x80 - b) << 10) >> 12);
    return (b * 0xf80) >> 12;
}
