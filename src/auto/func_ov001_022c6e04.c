/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_0208922c[];
s32 func_ov001_022c6944();
s32 func_ov001_022c6878();
void func_ov001_022c6d4c();
s32 func_ov001_022c7a34();

void func_ov001_022c6e04(s32 p0) {
    *(u16 *)((u8 *)data_0208922c + 0x48) = 0x10;
    func_ov001_022c6944(p0, data_0208922c, 0x10);
    func_ov001_022c6878(p0);
    func_ov001_022c7a34(p0, (u32)func_ov001_022c6d4c);
    *(volatile u32 *)0x4000000 = ((*(volatile u32 *)0x4000000 & ~0x1f00) | 0x300);
}
