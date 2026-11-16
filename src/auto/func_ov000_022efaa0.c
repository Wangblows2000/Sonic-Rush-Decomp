/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022f4ce4();
extern u8 data_ov000_0236ac60[];
s32 func_ov000_022f9188();
void func_ov000_022efa2c();

void func_ov000_022efaa0(u8 *p0) {
    func_ov000_022f4ce4(p0, 6, 0x1000, 1);
    s32 t0 = *(u32 *)data_ov000_0236ac60;
    s32 t1 = (*(u32 *)((u8 *)*(u32 *)data_ov000_0236ac60 + 0x32c) & 0x400);
    s32 v2 = (s32)data_ov000_0236ac60;
    s32 v3 = t1;
    if (t1 == 0) {
        *(u32 *)((u8 *)t0 + 0x10) = 0x1a000;
        s32 t4 = *(u32 *)v2;
        *(u32 *)((u8 *)t4 + 0x14) = 0x13e00;
        v2 = t4;
        v3 = 0x13e00;
    }
    func_ov000_022f9188(v2, v3, t0);
    *(u16 *)((u8 *)p0 + 0x434) = 0;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022efa2c;
}
