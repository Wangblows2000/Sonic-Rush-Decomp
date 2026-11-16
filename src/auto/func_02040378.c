/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_020405bc();
s32 func_020405e0();
extern u8 data_022c198c[];
s32 func_02040304();
s32 func_02040230();

void func_02040378(u8 *p0) {
    s32 t0 = *(u16 *)((u8 *)p0 + 0x2);
    if (t0 != 0) {
        func_020405bc(t0);
        func_020405e0(9);
        return;
    }
    s32 t1 = *(u32 *)data_022c198c;
    if (t1 != 0) {
        s32 t2 = func_02040304(t1);
        if (t2 != 0) {
            return;
        }
        func_020405e0(9);
        return;
    }
    s32 t3 = func_02040230();
    if (t3 != 0) {
        return;
    }
    func_020405e0(9);
}
