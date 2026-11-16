/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022cc00c();
extern u8 data_ov000_0236a444[];
s32 func_020366f4();
void func_ov000_022c7e68();

void func_ov000_022c7e80(u8 *p0) {
    func_ov000_022cc00c(((u8 *)p0 + 0x228), *(u32 *)((u8 *)p0 + 0x224), 0, 0x1000, 0);
    *(u32 *)((u8 *)data_ov000_0236a444 + 0xcc) = -0x3c000;
    func_020366f4(0, 0, 0x7e, data_ov000_0236a444);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022c7e68;
}
