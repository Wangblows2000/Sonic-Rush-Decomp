#include <nitro/types.h>
#include <NitroSDK/osthread.h>

void OSi_InsertThreadToList(UnkOSThread *t);
void OSi_RescheduleThread(void);

BOOL OS_SetThreadPriority(UnkOSThread *thread, u32 prio)
{
    UnkOSThread *t = data_0207d090.list;
    UnkOSThread *pre = NULL;
    u32 e = OS_DisableInterrupts();
    while (t != NULL && t != thread) {
        pre = t;
        t = t->next;
    }
    if (t == NULL || t == &data_0207d0e4) {
        OS_RestoreInterrupts(e);
        return FALSE;
    }
    if (t->priority != prio) {
        if (pre == NULL) {
            data_0207d090.list = thread->next;
        } else {
            pre->next = thread->next;
        }
        thread->priority = prio;
        OSi_InsertThreadToList(thread);
        OSi_RescheduleThread();
    }
    OS_RestoreInterrupts(e);
    return TRUE;
}
