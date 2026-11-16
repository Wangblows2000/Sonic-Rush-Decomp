/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022f4cbc();
extern u8 data_022c4560[];
s32 func_ov000_022f8c28();
s32 func_ov000_022f8bc8();
extern u8 data_ov000_02369ac0[];
void func_ov000_022f3544();

void func_ov000_022f3588(u8 *p0) {
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    func_ov000_022f4cbc(p0, 0);
    func_ov000_022f8c28(*(u32 *)data_022c4560, 0);
    func_ov000_022f8bc8(*(u32 *)data_022c4560, 0);
    *(u32 *)((u8 *)data_ov000_02369ac0 + 0x4) = 0x39;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022f3544;
}
