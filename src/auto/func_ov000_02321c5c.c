/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_02068df8();

void func_ov000_02321c5c(u8 *p0) {
    if ((*(u16 *)((u8 *)p0 + 0x5c) & 1) != 0) {
        s32 t0 = func_02068df8(*(s16 *)((u8 *)p0 + 0x2e), -4, 0x180);
        *(u16 *)((u8 *)p0 + 0x2e) = t0;
    } else {
        s32 t1 = func_02068df8(*(s16 *)((u8 *)p0 + 0x2e), 4, 0x180);
        *(u16 *)((u8 *)p0 + 0x2e) = t1;
    }
    s32 t2 = func_02068df8(*(s16 *)((u8 *)p0 + 0x30), -4, 0x240);
    *(u16 *)((u8 *)p0 + 0x30) = t2;
}
