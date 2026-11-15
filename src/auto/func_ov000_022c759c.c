/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022cc00c();
void func_ov000_022c7584();

void func_ov000_022c759c(u8 *p0) {
    func_ov000_022cc00c(((u8 *)p0 + 0x228), *(u32 *)((u8 *)p0 + 0x224), 4, 0x1000, 0);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022c7584;
}
