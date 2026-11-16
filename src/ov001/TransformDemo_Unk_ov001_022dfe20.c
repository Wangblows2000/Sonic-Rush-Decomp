#include <nitro/types.h>

extern u8 gDisplaySub[];
void GX_ResetBankForSubBG(void);
void GX_SetBankForSubOBJ(s32 a);
void GX_SetBankForLCDC(s32 a);
void GX_SetGraphicsMode(s32 a, s32 b, s32 c);
void GXS_SetGraphicsMode(s32 a);

void TransformDemo_Unk_ov001_022dfe20(void) {
    *(vu16 *)0x04000304 &= ~0x8000;
    GX_ResetBankForSubBG();
    *(u16 *)(gDisplaySub + 0x94) = 2;
    *(u16 *)(gDisplaySub + 0x92) = 0x80;
    *(u32 *)(gDisplaySub + 0x68) = 0x06860000;
    GX_SetBankForSubOBJ(8);
    GX_SetBankForLCDC(4);
    *(vu32 *)0x04000064 = 0x81320010;
    GX_SetGraphicsMode(1, 0, 1);
    *(vu32 *)0x04000000 = (*(vu32 *)0x04000000 & ~0x1f00) | 0x100;
    *(vu16 *)0x04000008 &= ~3;
    GXS_SetGraphicsMode(5);
    *(vu32 *)0x04001000 = (*(vu32 *)0x04001000 & ~0x1f00) | 0x1000;
}
