#include <nitro/types.h>
#include <NitroSDK/osthread.h>

void OSi_RescheduleThread(void);

void OS_SleepThread(u16 *queue)
{
    u32 e = OS_DisableInterrupts();
    UnkOSThread *cur = data_0207d084->current;
    if (queue != NULL) {
        *queue |= (u16)(1 << cur->id);
    }
    cur->state = 0;
    OSi_RescheduleThread();
    OS_RestoreInterrupts(e);
}
