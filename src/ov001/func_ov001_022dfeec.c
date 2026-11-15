#include <nitro/types.h>

void func_020030a8(void);
void func_0200330c(s32 a);
void GX_SetBankForLCDC(s32 a);
void func_02002ab0(s32 a, s32 b, s32 c);
void GXS_SetGraphicsMode(s32 a);

void func_ov001_022dfeec(void) {
    *(vu16 *)0x04000304 |= 0x8000;
    func_020030a8();
    func_0200330c(4);
    GX_SetBankForLCDC(8);
    *(vu32 *)0x04000064 = 0x81330010;
    func_02002ab0(1, 0, 1);
    *(vu32 *)0x04000000 = (*(vu32 *)0x04000000 & ~0x1f00) | 0x100;
    *(vu16 *)0x04000008 &= ~3;
    GXS_SetGraphicsMode(5);
    *(vu32 *)0x04001000 = (*(vu32 *)0x04001000 & ~0x1f00) | 0x400;
    *(vu16 *)0x0400100c = (*(vu16 *)0x0400100c & 0x43) | 0x4084;
    *(vu16 *)0x0400100c &= ~3;
    *(vu16 *)0x0400100c &= ~0x40;
}
