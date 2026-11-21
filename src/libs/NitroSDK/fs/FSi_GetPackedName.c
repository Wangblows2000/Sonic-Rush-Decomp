#include <nitro/types.h>

u32 FSi_GetPackedName(const char *name, s32 name_len)
{
    u32 ret = 0;
    if (name_len <= 3) {
        s32 i = 0;
        for (; i < name_len; ++i) {
            u32 c = (u8)name[i];
            if (!c) {
                break;
            }
            c = (u32)(c - 'A');
            if (c <= (u32)('Z' - 'A')) {
                c = (u32)(c + 'a');
            } else {
                c = (u32)(c + 'A');
            }
            ret |= (u32)(c << (i * 8));
        }
    }
    return ret;
}
