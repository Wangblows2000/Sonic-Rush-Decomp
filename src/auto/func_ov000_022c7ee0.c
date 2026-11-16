/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void func_ov000_022c7e80();

void func_ov000_022c7ee0(u8 *p0) {
    if ((*(u8 *)((u8 *)p0 + 0x334) & 2) == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022c7e80;
    ((s32 (*)())*(u32 *)((u8 *)p0 + 0x68))(p0, *(u32 *)((u8 *)p0 + 0x68));
}
