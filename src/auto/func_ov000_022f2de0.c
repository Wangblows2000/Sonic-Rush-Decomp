/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022f8fa0();
s32 func_ov000_022f3f8c();
s32 func_ov000_022f88dc();

void func_ov000_022f2de0(u8 *p0) {
    func_ov000_022f8fa0(((u8 *)p0 + 0x436), 3, 7);
    if ((*(u8 *)((u8 *)p0 + 0x57c) & 2) == 0) {
        return;
    }
    func_ov000_022f3f8c((*(u8 *)((u8 *)p0 + 0x57c) & 2));
    func_ov000_022f88dc();
}
