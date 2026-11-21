#include <nitro/types.h>
#include <main_sdk2/osthread.h>

void OSi_RescheduleThread(void);
u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);

static inline void WakeupMask(u32 mask)
{
    UnkOSThread *t = data_0207d090.list;
    while (t != NULL) {
        if (mask & (1 << t->id)) {
            t->state = 1;
        }
        t = t->next;
    }
}

void OS_WakeupThread(u16 *queue)
{
    u32 e = OS_DisableInterrupts();
    if (*queue != 0) {
        WakeupMask(*queue);
        *queue = 0;
        OSi_RescheduleThread();
    }
    OS_RestoreInterrupts(e);
}
