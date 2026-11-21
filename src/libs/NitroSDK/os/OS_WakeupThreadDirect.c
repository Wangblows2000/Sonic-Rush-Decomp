#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void OSi_RescheduleThread(void);
typedef struct { u8 pad[0x64]; u32 state; } UnkStruct_02005ba0;
void OS_WakeupThreadDirect(UnkStruct_02005ba0 *p) { u32 e = OS_DisableInterrupts(); p->state = 1; OSi_RescheduleThread(); OS_RestoreInterrupts(e); }
