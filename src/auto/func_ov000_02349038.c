/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_0234cb70();
s32 func_ov000_0234c4a4();
s32 func_0206cdf4();
s32 func_ov000_02346d10();
s32 func_ov000_02346aa8();
void func_ov000_02348f58();
s32 func_020446b0();
s32 func_0204b7ec();

void func_ov000_02349038(u8 *p0) {
    if ((*(u32 *)((u8 *)p0 + 0x32c) & 0x400) != 0) {
        return;
    }
    func_ov000_0234cb70(p0, (*(u32 *)((u8 *)p0 + 0x32c) & 0x400));
    func_ov000_0234c4a4(p0, 0x11);
    *(u16 *)((u8 *)p0 + 0x3b6) = 0;
    *(u16 *)((u8 *)p0 + 0x3b4) = 0;
    func_0206cdf4(((u8 *)p0 + 0x414), 0, 0, 0);
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) & ~0x40000);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) & ~1);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x100);
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x58) | 0x200);
    *(u32 *)((u8 *)p0 + 0x58) = t0;
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    s32 t1 = -*(s16 *)((u8 *)p0 + 0x370);
    *(u16 *)((u8 *)p0 + 0x30) = t1;
    func_ov000_02346d10(p0, t1, 0, t0);
    func_ov000_02346aa8(p0);
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) & ~0x30000000);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_02348f58;
    *(u32 *)((u8 *)p0 + 0xc) = 0;
    *(u16 *)((u8 *)p0 + 0x3ca) = 0;
    s32 t2 = (*(u32 *)((u8 *)p0 + 0x32c) | 0x400);
    *(u32 *)((u8 *)p0 + 0x32c) = t2;
    func_020446b0(4, t2);
    if ((*(u32 *)((u8 *)p0 + 0x32c) & 0x4000000) != 0) {
        func_0204b7ec(((u8 *)p0 + 0x384), 0, 0x52);
        return;
    }
    func_0204b7ec(((u8 *)p0 + 0x384), 0, 6);
}
