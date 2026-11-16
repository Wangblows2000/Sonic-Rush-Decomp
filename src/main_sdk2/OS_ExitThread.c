#include <nitro/types.h>
#include <main_sdk2/osthread.h>

void OSi_UnlockAllMutex(UnkOSThread *t);
void OSi_RemoveThreadFromList(UnkOSThread *t);
void OS_WakeupThread(void *queue);
void OSi_RescheduleThread(void);
void OS_Terminate(void);

void OS_ExitThread(void)
{
    UnkOSThread *cur;
    OS_DisableInterrupts();
    cur = data_0207d084->current;
    OSi_UnlockAllMutex(cur);
    OSi_RemoveThreadFromList(cur);
    data_0207d0a4[cur->id] = NULL;
    cur->state = 2;
    OS_WakeupThread(&cur->joinQueue);
    OSi_RescheduleThread();
    OS_Terminate();
}
