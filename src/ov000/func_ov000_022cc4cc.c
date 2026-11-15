#include <nitro/types.h>

void func_020027a4(s32 a, s32 b);

void func_ov000_022cc4cc(s32 x, s32 unused, s32 y, s32 flag) {
    s32 px = x << 4;
    s32 py = y << 4;
    s32 ox, oy;
    if (flag != 0) {
        ox = -0xaa8da;
        oy = 0x2a1290;
        func_020027a4(px + ox, py + oy);
    } else {
        ox = -0x117726;
        oy = 0x2a1290;
        func_020027a4(px + ox, py + oy);
    }
}
