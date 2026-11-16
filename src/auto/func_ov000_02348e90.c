/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_0234cb70();
s32 Player_SetAnimation();
s32 func_ov000_02346d10();
s32 func_ov000_02346aa8();
void func_ov000_02348dc0();
s32 ScreenShake_Start();

void func_ov000_02348e90(u8 *p0) {
    if ((*(u32 *)((u8 *)p0 + 0x32c) & 0x400) != 0) {
        return;
    }
    if ((*(u32 *)((u8 *)p0 + 0x32c) & 0x1000000) != 0) {
        return;
    }
    func_ov000_0234cb70(p0, (*(u32 *)((u8 *)p0 + 0x32c) & 0x1000000), *(u32 *)((u8 *)p0 + 0x32c));
    Player_SetAnimation(p0, 0x11);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) & ~1);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x100);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x2000);
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x58) | 0x200);
    *(u32 *)((u8 *)p0 + 0x58) = t0;
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    *(u16 *)((u8 *)p0 + 0x30) = 0;
    func_ov000_02346d10(p0, 0, t0);
    func_ov000_02346aa8(p0);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_02348dc0;
    *(u32 *)((u8 *)p0 + 0xc) = 0;
    *(u16 *)((u8 *)p0 + 0x3ca) = 0;
    *(u16 *)((u8 *)p0 + 0x44) = 0x100;
    *(u16 *)((u8 *)p0 + 0x46) = 0x100;
    s32 t1 = (*(u32 *)((u8 *)p0 + 0x32c) | 0x400);
    *(u32 *)((u8 *)p0 + 0x32c) = t1;
    ScreenShake_Start(4, t1);
}
