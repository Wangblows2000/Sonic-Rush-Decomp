#include <nitro/types.h>

#pragma thumb on
void func_02061cd4(u8 x, u8 y, u8 w, u8 h, u16 *map) {
    u8 r;
    u8 j;
    s32 oa;
    s32 ob;
    u16 *p;
    u16 t;

    for (r = x + w - 1; x < r; x++, r--) {
        for (j = y; j < y + h; j++) {
            if (x < 32)
                oa = ((j << 5) + x) << 1;
            else
                oa = (((j << 5) + (x - 32)) << 1) + 0x800;
            p = (u16 *)((u8 *)map + oa);
            if (r < 32)
                ob = ((j << 5) + r) << 1;
            else
                ob = (((j << 5) + (r - 32)) << 1) + 0x800;
            t = *p;
            *p = *(u16 *)((u8 *)map + ob);
            *(u16 *)((u8 *)map + ob) = t;
        }
    }
}
#pragma thumb reset
