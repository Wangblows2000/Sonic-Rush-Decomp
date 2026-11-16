/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02369acc[];
s32 func_020366f4();
void func_ov000_022c759c();

void func_ov000_022c75dc(u8 *p0) {
    u8 t0 = *(u8 *)((u8 *)p0 + 0x334);
    u32 t1 = *(u32 *)data_ov000_02369acc;
    if ((t0 & 2) == 0) {
        return;
    }
    u32 t2 = *(u32 *)((u8 *)t1 + 0x20c);
    *(u32 *)((u8 *)t1 + 0x20c) = (t2 & ~1);
    func_020366f4(0, 0, 0xea, (t2 & ~1));
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022c759c;
}
