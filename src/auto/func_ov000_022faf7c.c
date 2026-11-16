/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022fb898();
s32 func_ov000_022fb738();

void func_ov000_022faf7c(u8 *p0) {
    *(u8 *)((u8 *)p0 + 0x1) = (*(u8 *)((u8 *)p0 + 0x1) + 0x10);
    s32 t0 = *(u32 *)((u8 *)p0 + 0xc);
    if (t0 == 0) {
        func_ov000_022fb898(p0, t0);
        func_ov000_022fb738(p0);
        *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
    }
    *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) - 0x1);
}
