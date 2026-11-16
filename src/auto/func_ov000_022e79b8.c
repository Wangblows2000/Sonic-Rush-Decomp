/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022e7b6c();
void func_ov000_022e78f0();

void func_ov000_022e79b8(u8 *p0) {
    func_ov000_022e7b6c();
    s32 t0 = *(u16 *)((u8 *)p0 + 0x46a);
    *(u16 *)((u8 *)p0 + 0x46a) = (*(u16 *)((u8 *)p0 + 0x46a) + 0x1);
    if ((u32)t0 <= (u32)0x1e) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x440) = (u32)func_ov000_022e78f0;
    ((s32 (*)())*(u32 *)((u8 *)p0 + 0x440))(p0, *(u32 *)((u8 *)p0 + 0x440), t0);
}
