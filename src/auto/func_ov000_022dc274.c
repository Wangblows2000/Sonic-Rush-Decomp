/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022db848();
void func_ov000_022dc20c();

void func_ov000_022dc274(u8 *p0) {
    s32 t0 = func_ov000_022db848();
    *(u32 *)((u8 *)p0 + 0x348) = t0;
    *(u32 *)((u8 *)p0 + 0x3c0) = (u32)func_ov000_022dc20c;
    *(u32 *)((u8 *)p0 + 0x3c4) = 0;
    *(u32 *)((u8 *)p0 + 0x3a8) = 0;
    *(u32 *)((u8 *)p0 + 0x3ac) = 0;
    *(u32 *)((u8 *)p0 + 0x3b0) = 0;
    *(u32 *)((u8 *)p0 + 0x3b4) = 0;
    *(u32 *)((u8 *)p0 + 0x3b8) = 0;
    *(u32 *)((u8 *)p0 + 0x3bc) = 0;
}
