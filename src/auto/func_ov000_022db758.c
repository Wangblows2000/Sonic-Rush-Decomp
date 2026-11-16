/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022c5e80();

void func_ov000_022db758(u8 *p0, s32 p1, s32 p2, s32 p3) {
    if ((s32)p0 != 0) {
        func_ov000_022c5e80(0x128fb8, 0x749fed, ((s32)p2 << 4), p0, ((u8 *)p0 + 0x8), p1);
        *(u32 *)((u8 *)p0 + 0x4) = (-(s32)p3 << 4);
        return;
    }
    func_ov000_022c5e80(0x128fb8, 0x749fed, ((s32)p2 << 4), 0, 0, p1);
}
