#include <nitro/types.h>

extern u8 data_02087d04[];
void func_020030bc(void);
void func_02003290(s32 a);
void GX_SetBankForLCDC(s32 a);
void func_02002ab0(s32 a, s32 b, s32 c);
void GXS_SetGraphicsMode(s32 a);

void func_ov001_022dfe20(void) {
    *(vu16 *)0x04000304 &= ~0x8000;
    func_020030bc();
    *(u16 *)(data_02087d04 + 0x94) = 2;
    *(u16 *)(data_02087d04 + 0x92) = 0x80;
    *(u32 *)(data_02087d04 + 0x68) = 0x06860000;
    func_02003290(8);
    GX_SetBankForLCDC(4);
    *(vu32 *)0x04000064 = 0x81320010;
    func_02002ab0(1, 0, 1);
    *(vu32 *)0x04000000 = (*(vu32 *)0x04000000 & ~0x1f00) | 0x100;
    *(vu16 *)0x04000008 &= ~3;
    GXS_SetGraphicsMode(5);
    *(vu32 *)0x04001000 = (*(vu32 *)0x04001000 & ~0x1f00) | 0x1000;
}
