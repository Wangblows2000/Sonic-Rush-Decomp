/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_0200ea14();
extern u8 data_0207f09c[];
s32 func_0200e2d0();

s32 func_0200e6c4(s32 p0, s32 p1, s32 p2) {
    s32 t0 = func_0200ea14();
    if (t0 == 0) {
        return 1;
    }
    *(u32 *)((u8 *)data_0207f09c + 0x4) = p1;
    *(u32 *)((u8 *)data_0207f09c + 0x8) = p2;
    func_0200e2d0(((((u32)(s32)p0 >> 0x10) & 0xff) | 0x2006300), data_0207f09c);
    func_0200e2d0((((s32)p0 & 0xffff) | 0x1010000));
    return 0;
}
