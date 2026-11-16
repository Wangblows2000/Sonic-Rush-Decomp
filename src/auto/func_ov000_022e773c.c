/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02369b78[];
s32 func_ov000_022ee198();
void func_ov000_022e98d8();

void func_ov000_022e773c(u8 *p0) {
    if ((*(u32 *)((u8 *)p0 + 0x58) & 1) == 0) {
        return;
    }
    s32 t0 = *(u32 *)data_ov000_02369b78;
    *(u32 *)((u8 *)t0 + 0x210) = 1;
    func_ov000_022ee198(p0, 0x31, t0, 1);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x10);
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    *(u16 *)((u8 *)p0 + 0x30) = 0;
    *(u32 *)((u8 *)p0 + 0x440) = (u32)func_ov000_022e98d8;
}
