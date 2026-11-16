#include <nitro/types.h>

s32 func_0200abcc(const char *str1, const char *str2, s32 len)
{
    s32 i;
    for (i = 0; i < len; ++i) {
        u32 c = (u32)((u8)str1[i] - 'A');
        u32 d = (u32)((u8)str2[i] - 'A');
        if (c <= (u32)('Z' - 'A')) {
            c += 'a' - 'A';
        }
        if (d <= (u32)('Z' - 'A')) {
            d += 'a' - 'A';
        }
        if (c != d) {
            return (s32)(c - d);
        }
    }
    return 0;
}
