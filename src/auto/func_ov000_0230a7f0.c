/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_02068db4();

void func_ov000_0230a7f0(u8 *p0) {
    if ((*(u32 *)((u8 *)p0 + 0x58) & 0x80) == 0) {
        s32 t0 = func_02068db4(*(s16 *)((u8 *)p0 + 0x2e), 8);
        *(u16 *)((u8 *)p0 + 0x2e) = t0;
        s32 t1 = func_02068db4(*(s16 *)((u8 *)p0 + 0x30), 8);
        *(u16 *)((u8 *)p0 + 0x30) = t1;
    }
    s32 t2 = (*(u16 *)((u8 *)p0 + 0x5c) & 8);
    if (t2 != 0) {
        *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
    }
}
