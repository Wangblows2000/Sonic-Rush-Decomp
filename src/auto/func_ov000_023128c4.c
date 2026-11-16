/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_023126e8();
s32 func_ov000_02312634();
s32 func_0201bea8();
s32 func_0204b774();
extern u8 data_ov000_0236ac60[];
s32 func_02068204();

void func_ov000_023128c4(u8 *p0) {
    func_ov000_023126e8();
    func_ov000_02312634(p0);
    s32 t0 = func_0201bea8(0, 0x67);
    if (t0 == 0) {
        func_0204b774(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x67, 0x20, -1);
    }
    func_02068204(*(u32 *)((u8 *)p0 + 0x1dc), p0, *(u32 *)data_ov000_0236ac60);
}
