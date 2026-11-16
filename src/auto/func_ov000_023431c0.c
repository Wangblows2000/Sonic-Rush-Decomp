/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c4560[];
extern u8 data_022c4590[];
s32 func_0204b8e0();

void func_ov000_023431c0(s32 p0, s32 p1) {
    if ((u32)(*(u32 *)((u8 *)data_022c4560 + 0x10) - 0x1) <= (u32)1) {
        return;
    }
    *(u8 *)((u8 *)data_022c4590 + 0x14) = (*(u8 *)((u8 *)data_022c4590 + 0x14) + (s32)p1);
    s32 t0 = *(u8 *)((u8 *)data_022c4590 + 0x14);
    s32 v1 = t0;
    if ((u32)t0 > (u32)0x63) {
        v1 = 0x63;
    }
    *(u8 *)((u8 *)data_022c4590 + 0x14) = v1;
    func_0204b8e0(0, 0x18, data_022c4590, v1);
}
