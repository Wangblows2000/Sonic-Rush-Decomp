/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_020366f4();
void func_ov000_022c7c70();

void func_ov000_022c7ccc(u8 *p0) {
    if ((*(u8 *)((u8 *)p0 + 0x334) & 2) == 0) {
        return;
    }
    s32 t0 = (*(u16 *)((u8 *)p0 + 0x5c) | 0x20);
    *(u16 *)((u8 *)p0 + 0x5c) = t0;
    func_020366f4(0, 0, 0xd1, t0);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022c7c70;
}
