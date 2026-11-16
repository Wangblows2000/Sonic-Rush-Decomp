#include <nitro/types.h>

u32 RtcBCD2HEX(u32 bcd)
{
    u32 result = 0;
    u32 mul;
    s32 i;

    for (i = 0; i < 8; i++) {
        if (((bcd >> (i * 4)) & 0xf) >= 10) {
            return 0;
        }
    }
    mul = 1;
    for (i = 0; i < 8; i++) {
        result += ((bcd >> (i * 4)) & 0xf) * mul;
        mul *= 10;
    }
    return result;
}
