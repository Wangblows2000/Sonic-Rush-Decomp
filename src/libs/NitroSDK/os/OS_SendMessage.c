#include <nitro/types.h>
#include <NitroSDK/osmsg.h>

BOOL OS_SendMessage(UnkOSMessageQueue *q, void *msg, s32 flags)
{
    u32 e = OS_DisableInterrupts();
    while (q->msgCount <= q->usedCount) {
        if (!(flags & 1)) {
            OS_RestoreInterrupts(e);
            return FALSE;
        }
        OS_SleepThread(&q->queueSend);
    }
    q->msgArray[(q->firstIndex + q->usedCount) % q->msgCount] = msg;
    q->usedCount++;
    OS_WakeupThread(&q->queueReceive);
    OS_RestoreInterrupts(e);
    return TRUE;
}
