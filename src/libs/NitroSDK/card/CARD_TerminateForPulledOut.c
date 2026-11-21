#include <nitro/types.h>

s32 PM_ForceToPowerOff(void);
void CARDi_SendtoPxi(u32 a, u32 b);
void OS_Terminate(void);
void CARD_TerminateForPulledOut(void) { if ((*(vu16 *)0x27fffa8 & 0x8000) >> 15) PM_ForceToPowerOff(); CARDi_SendtoPxi(1, 1); OS_Terminate(); }
