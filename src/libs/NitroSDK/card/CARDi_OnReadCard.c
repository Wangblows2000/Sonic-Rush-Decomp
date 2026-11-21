/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_0207f2c0[];
s32 MI_StopDma();
s32 OS_DisableIrqMask();
s32 OS_ResetRequestIrqMask();
s32 CARDi_ReadEnd();
s32 CARDi_SetCardDma();

void CARDi_OnReadCard(void) {
    MI_StopDma(*(u32 *)((u8 *)data_0207f2c0 + 0x24));
    s32 t0 = (*(u32 *)((u8 *)data_0207f2c0 + 0x18) + 0x200);
    s32 t1 = (*(u32 *)((u8 *)data_0207f2c0 + 0x1c) + 0x200);
    s32 t2 = (*(u32 *)((u8 *)data_0207f2c0 + 0x20) - 0x200);
    *(u32 *)((u8 *)data_0207f2c0 + 0x18) = t0;
    *(u32 *)((u8 *)data_0207f2c0 + 0x1c) = t1;
    *(u32 *)((u8 *)data_0207f2c0 + 0x20) = t2;
    if (t2 == 0) {
        OS_DisableIrqMask(0x80000, t2, t1, t0);
        OS_ResetRequestIrqMask(0x80000);
        CARDi_ReadEnd();
        return;
    }
    CARDi_SetCardDma();
}
