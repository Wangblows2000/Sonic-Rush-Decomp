/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_0201c070();
s32 func_020683e4();
s32 func_ov000_022fb9c8();

void func_ov000_02312954(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    func_0201c070(*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x10) + 0x1dc), 0);
    func_020683e4(*(u32 *)((u8 *)t0 + 0x29c), 0);
    func_020683e4((*(u32 *)((u8 *)t0 + 0x29c) + 0x8), 1);
    func_ov000_022fb9c8(p0);
}
