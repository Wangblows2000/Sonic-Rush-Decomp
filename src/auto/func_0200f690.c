/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_0207f2c0[];
s32 func_02005ba0();

void func_0200f690(s32 p0, s32 p1, s32 p2) {
    if ((s32)p0 != 0xb) {
        return;
    }
    if ((s32)p2 == 0) {
        return;
    }
    *(u32 *)((u8 *)data_0207f2c0 + 0x34) = (*(u32 *)((u8 *)data_0207f2c0 + 0x34) & ~0x20);
    func_02005ba0(*(u32 *)((u8 *)data_0207f2c0 + 0xd0), data_0207f2c0);
}
