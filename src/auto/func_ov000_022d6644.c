/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_0201c070();
s32 func_0201bf1c();
void func_ov000_022d6568();

void func_ov000_022d6644(u8 *p0) {
    *(u16 *)((u8 *)p0 + 0x30) = 0;
    *(u32 *)((u8 *)p0 + 0x14) = 0x20000;
    func_0201c070(*(u32 *)((u8 *)p0 + 0x2c0), 0x20);
    func_0201bf1c(*(u32 *)((u8 *)p0 + 0x2c0));
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022d6568;
}
