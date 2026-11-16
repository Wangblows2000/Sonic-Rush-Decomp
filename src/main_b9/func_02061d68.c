#include <nitro/types.h>

void func_02061cd4(u8 x, u8 y, u8 w, u8 h, void *map);

#pragma thumb on
void func_02061d68(u8 x, u8 y, u8 w, u8 h, void *map, u8 flag) {
    u16 *m;
    u16 v;
    s32 idx;
    u8 n;
    u8 i;
    u8 j;
    u16 *p;
    u8 sx;
    s32 end;

    m = map;
    if (x < 32)
        idx = ((y << 5) + x) << 1;
    else
        idx = (((y << 5) + (x - 32)) << 1) + 0x800;
    v = *(u16 *)((u8 *)m + idx);
    if (flag && (v & 0x400))
        return;
    if (!flag && !(v & 0x400))
        return;
    func_02061cd4(x, y, w, h, map);
    if (x < 32) {
        if (x + w < 32)
            n = w;
        else
            n = 32 - x;
        for (j = y; j < y + h; j++) {
            p = &m[(j << 5) + x];
            for (i = x; i < x + n; i++) {
                *p++ ^= 0x400;
            }
        }
    }
    if (x + w > 32) {
        if (x > 32) {
            sx = x;
        } else {
            sx = 32;
            w -= 32 - x;
        }
        end = y + h;
        sx -= 32;
        for (; y < end; y++) {
            p = (u16 *)((u8 *)m + 0x800) + ((y << 5) + sx);
            for (i = sx; i < sx + w; i++) {
                *p++ ^= 0x400;
            }
        }
    }
}
#pragma thumb reset
