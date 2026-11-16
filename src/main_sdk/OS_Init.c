#include <nitro/types.h>

void OS_InitArena(void); void PXI_Init(void); void OS_InitLock(void); void OS_InitArenaEx(void);
void OS_InitIrqTable(void); void OS_InitException(void); void MI_Init(void); void OS_InitVAlarm(void);
void OSi_InitVramExclusive(void); void OS_InitThread(void); void OS_InitReset(void); void CTRDG_Init(void);
void CARD_Init(void); void PM_Init(void);
void OS_Init(void) {
    OS_InitArena(); PXI_Init(); OS_InitLock(); OS_InitArenaEx(); OS_InitIrqTable(); OS_InitException(); MI_Init();
    OS_InitVAlarm(); OSi_InitVramExclusive(); OS_InitThread(); OS_InitReset(); CTRDG_Init(); CARD_Init(); PM_Init();
}
