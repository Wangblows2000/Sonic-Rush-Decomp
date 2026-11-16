#include <nitro/types.h>
#include <main_sdk2/osmsg.h>

BOOL func_020064a0(UnkOSMessageQueue *q, void *msg, s32 flags)
{
    u32 e = OS_DisableInterrupts();
    while (q->msgCount <= q->usedCount) {
        if (!(flags & 1)) {
            OS_RestoreInterrupts(e);
            return FALSE;
        }
        func_02005c48(&q->queueSend);
    }
    q->msgArray[(q->firstIndex + q->usedCount) % q->msgCount] = msg;
    q->usedCount++;
    func_02005bd4(&q->queueReceive);
    OS_RestoreInterrupts(e);
    return TRUE;
}
