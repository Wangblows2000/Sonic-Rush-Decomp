#include <nitro/types.h>
#include <main_sdk2/osmsg.h>

BOOL func_020063f4(UnkOSMessageQueue *q, void **msg, s32 flags)
{
    u32 e = OS_DisableInterrupts();
    while (q->usedCount == 0) {
        if (!(flags & 1)) {
            OS_RestoreInterrupts(e);
            return FALSE;
        }
        func_02005c48(&q->queueReceive);
    }
    if (msg != NULL) {
        *msg = q->msgArray[q->firstIndex];
    }
    q->firstIndex = (q->firstIndex + 1) % q->msgCount;
    q->usedCount--;
    func_02005bd4(&q->queueSend);
    OS_RestoreInterrupts(e);
    return TRUE;
}
