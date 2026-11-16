/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_020405bc();
extern u8 data_022c196c[];
s32 func_0204045c();

void func_0203eaa0(u8 *p0) {
    s32 t0 = *(u16 *)((u8 *)p0 + 0x2);
    if (t0 != 0) {
        if (t0 != 0xf) {
            func_020405bc(t0);
            return;
        }
    }
    s32 t1 = *(u32 *)((u8 *)p0 + 0x20);
    if (t1 != 0) {
        ((s32 (*)())t1)((t0 == 0), t1);
    }
    if ((u32)(*(u32 *)data_022c196c - 0x6) > (u32)1) {
        return;
    }
    func_0204045c(p0);
}
