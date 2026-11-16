/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 OS_Terminate();
s32 OS_GetLockID();
s32 CARD_LockRom();
s32 MI_StopDma();
s32 OS_SetIrqMask();
s32 OS_ResetRequestIrqMask();
s32 OSi_SendToPxi();
s32 OSi_DoResetSystem();

void OS_ResetSystem(s32 p0) {
    s32 t0 = *(u16 *)0x27ffc40;
    if (t0 == 2) {
        OS_Terminate(t0, 0x27ffc40);
    }
    s32 t1 = OS_GetLockID();
    CARD_LockRom(((u32)(t1 << 0x10) >> 0x10));
    MI_StopDma(0);
    MI_StopDma(1);
    MI_StopDma(2);
    MI_StopDma(3);
    OS_SetIrqMask(0x40000);
    OS_ResetRequestIrqMask(-1);
    *(u32 *)0x27ffc20 = p0;
    OSi_SendToPxi(0x10, 0x27ffc20);
    OSi_DoResetSystem();
}
