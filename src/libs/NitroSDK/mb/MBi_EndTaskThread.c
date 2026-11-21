#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
BOOL MBi_IsTaskAvailable(void);
extern u8 *data_02080f40;
void MBi_SetTask(void *p, u32 a, u32 b, u32 c);
void MBi_EndTaskThread(u32 v) {
    u32 e = OS_DisableInterrupts();
    if (MBi_IsTaskAvailable()) MBi_SetTask(data_02080f40 + 0x98, 0, v, 0);
    OS_RestoreInterrupts(e);
}
