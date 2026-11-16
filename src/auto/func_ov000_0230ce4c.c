/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_0230d1ac();
extern u8 data_ov000_0236a00c[];

void func_ov000_0230ce4c(u8 *p0, u8 *p1) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x18);
    s32 t1 = *(u32 *)((u8 *)p1 + 0x18);
    if (t1 == 0) {
        return;
    }
    if (*(u8 *)t1 != 1) {
        return;
    }
    *(u16 *)((u8 *)t0 + 0x62) = 0x88;
    func_ov000_0230d1ac(t0, 0x88);
    *(u32 *)data_ov000_0236a00c = 0;
}
