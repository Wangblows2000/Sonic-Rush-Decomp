#include <nitro/types.h>

extern u16 data_02072e70[];
extern s32 data_02072fc8[];
extern s32 data_02072ee4[];

s32 func_0203422c(u32 code)
{
    s32 lo = 0;
    s32 hi = 0x38;
    s32 d;
    do {
        s32 s = lo + hi;
        s32 mid = (s & 1) + (s >> 1);
        if (data_02072e70[mid] <= code)
            lo = mid;
        else
            hi = mid - 1;
    } while (lo < hi);
    d = code - data_02072e70[lo];
    if (d >= data_02072fc8[lo])
        return 0;
    return d + data_02072ee4[lo];
}
