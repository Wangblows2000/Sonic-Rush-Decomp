/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022f4ce4();
extern u8 data_ov000_0236ac60[];
s32 func_ov000_022fad3c();
s32 func_020366f4();
void func_ov000_022f10dc();

void func_ov000_022f112c(u8 *p0) {
    func_ov000_022f4ce4(p0, 0x2a, 0x1000, 1);
    func_ov000_022fad3c(0xde, *(u32 *)((u8 *)*(u32 *)data_ov000_0236ac60 + 0x10), (*(u32 *)((u8 *)*(u32 *)data_ov000_0236ac60 + 0x14) - 0x8000), ((u32)(*(u32 *)((u8 *)p0 + 0x458) << 0x10) >> 0x10), 0, 0, 0, 0, 0);
    func_020366f4(0, 0, 0xd1);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022f10dc;
}
