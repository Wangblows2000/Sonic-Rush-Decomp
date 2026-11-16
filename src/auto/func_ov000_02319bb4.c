/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236aad8[];
void func_ov000_02319920();
s32 func_0201c070();

void func_ov000_02319bb4(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x80);
    *(u16 *)((u8 *)p0 + 0x30) = 0;
    *(u32 *)((u8 *)p0 + 0x14) = (*(u16 *)((u8 *)data_ov000_0236aad8 + 0x62) << 8);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_02319920;
    *(u16 *)((u8 *)p0 + 0x34) = 0x100;
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    *(u8 *)((u8 *)p0 + 0x1) = 0;
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x820000);
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x58) | 0x100);
    *(u32 *)((u8 *)p0 + 0x58) = t0;
    *(u16 *)((u8 *)p0 + 0x428) = 0x100;
    func_0201c070(*(u32 *)((u8 *)p0 + 0x1dc), 0, ((u8 *)p0 + 0x400), t0);
}
