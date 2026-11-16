/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_023122d8();

void func_ov001_02311064(u8 *p0) {
    if ((*(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0xac) + 0x10c) & 2) == 0) {
        return;
    }
    func_ov001_023122d8(p0, 0x15);
}
