/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022f4ce4();
s32 func_ov000_022f9044();
s32 func_ov000_022f4cbc();
s32 func_ov000_022f9188();
extern u8 data_ov000_02369ac0[];
void func_ov000_022ef908();

void func_ov000_022ef9c8(u8 *p0) {
    func_ov000_022f4ce4(p0, 0x24, 0x1000, 0);
    func_ov000_022f9044(p0);
    func_ov000_022f4cbc(p0);
    *(u32 *)((u8 *)p0 + 0x1b8) = 0;
    func_ov000_022f9188(0);
    *(u16 *)((u8 *)p0 + 0x434) = 0;
    *(u32 *)((u8 *)data_ov000_02369ac0 + 0x4) = 0x3a;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022ef908;
}
