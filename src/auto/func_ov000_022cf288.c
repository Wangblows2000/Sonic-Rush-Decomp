/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void func_ov000_022cf1d8();
s32 func_020446b0();

void func_ov000_022cf288(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) - 0x1);
    if (*(u32 *)((u8 *)p0 + 0xc) != 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x4) = (u32)func_ov000_022cf1d8;
    func_020446b0(9, (u32)func_ov000_022cf1d8);
    *(u32 *)((u8 *)p0 + 0xc) = 0x78;
}
