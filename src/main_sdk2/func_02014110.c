#include <nitro/types.h>
#include <main_sdk2/blkinfo.h>

s32 func_02014110(UnkBlockInfo *info)
{
    s32 i = info->cursor + 1;
    s32 start;
    s32 b;
    u32 *p;
    s32 n = info->count;
    u32 *bits;

    if (i >= n) {
        i = 0;
    }
    bits = info->bits;
    p = &bits[i >> 5];
    start = i;
    b = i & 0x1f;
    do {
        if (!(*p & (1 << b))) {
            goto found;
        }
        if (++i < n) {
            if (++b > 31) {
                b = 0;
                p++;
            }
        } else {
            i = 0;
            b = 0;
            p = bits;
        }
    } while (i != start);
    goto notfound;
found:
    info->last = i;
    return i;
notfound:
    return -1;
}
