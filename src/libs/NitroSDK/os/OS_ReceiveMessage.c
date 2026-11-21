#include <nitro/types.h>
#include <main_sdk2/osmsg.h>

BOOL OS_ReceiveMessage(UnkOSMessageQueue *q, void **msg, s32 flags)
{
    u32 e = OS_DisableInterrupts();
    while (q->usedCount == 0) {
        if (!(flags & 1)) {
            OS_RestoreInterrupts(e);
            return FALSE;
        }
        OS_SleepThread(&q->queueReceive);
    }
    if (msg != NULL) {
        *msg = q->msgArray[q->firstIndex];
    }
    q->firstIndex = (q->firstIndex + 1) % q->msgCount;
    q->usedCount--;
    OS_WakeupThread(&q->queueSend);
    OS_RestoreInterrupts(e);
    return TRUE;
}
