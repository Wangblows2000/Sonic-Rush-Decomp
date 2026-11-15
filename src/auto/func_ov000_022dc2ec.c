/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void func_ov000_022dc2b8();

void func_ov000_022dc2ec(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x3c0) = (u32)func_ov000_022dc2b8;
    *(u32 *)((u8 *)p0 + 0x3c4) = 0;
    *(u32 *)((u8 *)p0 + 0x3a8) = 0;
    *(u32 *)((u8 *)p0 + 0x3ac) = 0;
    *(u32 *)((u8 *)p0 + 0x3b0) = 0;
    *(u32 *)((u8 *)p0 + 0x3b4) = 0x1400;
    *(u32 *)((u8 *)p0 + 0x3b8) = 0x80;
    *(u32 *)((u8 *)p0 + 0x3bc) = 0x1800;
}
