/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_022f3b50();
s32 func_ov001_022f6450();

void func_ov001_022f61ec(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x1c8) = (*(u32 *)((u8 *)p0 + 0x1c8) - 0x1);
    s32 t0 = *(u32 *)((u8 *)p0 + 0x1c8);
    if (t0 > 0) {
        func_ov001_022f3b50(p0, t0);
        return;
    }
    func_ov001_022f6450();
}
