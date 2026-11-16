/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_0204b7ec();

void func_ov000_02305e28(u8 *p0) {
    if ((s32)p0 == 0) {
        return;
    }
    if ((*(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x4) & 8) != 0) {
        return;
    }
    if ((*(u32 *)((u8 *)p0 + 0x54) & 0x1000) != 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x1f4) = (*(u32 *)((u8 *)p0 + 0x1f4) | 1);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) & ~0x200);
    func_0204b7ec(0, 0, 0x6a);
}
