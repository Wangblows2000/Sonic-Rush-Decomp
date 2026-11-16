/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022f4ce4();
s32 func_02036754();
s32 func_020366f4();
void func_ov000_022f052c();

void func_ov000_022f05d0(u8 *p0) {
    func_ov000_022f4ce4(p0, 0x21, 0x1000, 1);
    func_02036754(0, 0, 0xc7);
    func_020366f4(0, 0, 0xd0);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022f052c;
}
