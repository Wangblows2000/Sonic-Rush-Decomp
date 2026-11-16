#include <nitro/types.h>
#include <main_sdk2/osmsg.h>

BOOL func_02006340(UnkOSMessageQueue *q, void *msg, s32 flags)
{
    u32 e = OS_DisableInterrupts();
    while (q->msgCount <= q->usedCount) {
        if (!(flags & 1)) {
            OS_RestoreInterrupts(e);
            return FALSE;
        }
        func_02005c48(&q->queueSend);
    }
    q->firstIndex = (q->firstIndex + q->msgCount - 1) % q->msgCount;
    q->msgArray[q->firstIndex] = msg;
    q->usedCount++;
    func_02005bd4(&q->queueReceive);
    OS_RestoreInterrupts(e);
    return TRUE;
}
