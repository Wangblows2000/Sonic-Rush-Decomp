#include <nitro/types.h>
#include <main_sdk2/osmsg.h>

BOOL OS_JamMessage(UnkOSMessageQueue *q, void *msg, s32 flags)
{
    u32 e = OS_DisableInterrupts();
    while (q->msgCount <= q->usedCount) {
        if (!(flags & 1)) {
            OS_RestoreInterrupts(e);
            return FALSE;
        }
        OS_SleepThread(&q->queueSend);
    }
    q->firstIndex = (q->firstIndex + q->msgCount - 1) % q->msgCount;
    q->msgArray[q->firstIndex] = msg;
    q->usedCount++;
    OS_WakeupThread(&q->queueReceive);
    OS_RestoreInterrupts(e);
    return TRUE;
}
