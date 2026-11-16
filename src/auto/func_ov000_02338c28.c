/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gCamera[];

void func_ov000_02338c28(u8 *p0) {
    if ((*(u32 *)((u8 *)gCamera + 0xc8) & 0x100) != 0) {
        return;
    }
    s32 t0 = *(u32 *)((u8 *)p0 + 0xc);
    if (t0 == 0) {
        return;
    }
    ((s32 (*)())t0)(p0, t0);
}
