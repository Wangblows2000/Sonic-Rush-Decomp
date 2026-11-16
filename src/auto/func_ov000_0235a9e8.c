/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022c5e80();
s32 Model_Draw();

void func_ov000_0235a9e8(u8 *p0, u8 *p1) {
    func_ov000_022c5e80(0x128fb8, 0x749fed, (*(u32 *)p1 << 4), ((u8 *)p0 + 0xec), ((u8 *)p0 + 0xf4), 0);
    *(u32 *)((u8 *)p0 + 0xf0) = (-*(u32 *)((u8 *)p1 + 0x4) << 4);
    Model_Draw(((u8 *)p0 + 0xa4));
}
