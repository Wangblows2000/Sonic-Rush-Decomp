/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c4560[];
extern u8 data_022c44f4[];
s32 func_0201be40();
s32 func_0201be10();
extern u8 data_0225044c[];
extern u8 data_022c44f0[];

void func_0204c66c(s32 p0) {
    if ((u32)*(u8 *)((u8 *)data_022c4560 + 0x1c) >= (u32)9) {
        return;
    }
    if ((u32)*(u8 *)((u8 *)data_022c4560 + 0x1d) >= (u32)2) {
        return;
    }
    if ((s32)p0 != 0) {
        func_0201be40(data_022c44f4, 0, 0);
        func_0201be10(data_022c44f4, 0);
        func_0201be40(data_022c44f0, *(u8 *)data_0225044c, 0);
        func_0201be10(data_022c44f0, 0x40);
        return;
    }
    func_0201be40(data_022c44f4, *(u8 *)data_0225044c, 0);
    func_0201be10(data_022c44f4, 0x40);
    func_0201be40(data_022c44f0, 0, 0);
    func_0201be10(data_022c44f0, 0);
}
