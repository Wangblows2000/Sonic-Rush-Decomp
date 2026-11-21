#include <nitro/types.h>
#include <NitroSDK/task.h>

BOOL MBi_IsTaskAvailable(void);
void OS_Terminate(void);
u32 OS_GetThreadPriority(void *thread);
u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void OS_WakeupThreadDirect(void *thread);

void MBi_SetTask(UnkTask_0201a2e8 *task, void (*func)(UnkTask_0201a2e8 *), void (*done)(UnkTask_0201a2e8 *), u32 priority)
{
    UnkTaskCtx_0201a2e8 *ctx = data_02080f40;
    u32 e;

    if (!MBi_IsTaskAvailable()) {
        OS_Terminate();
    }
    if (task->busy) {
        OS_Terminate();
    }
    if (priority > 31) {
        u32 cur = OS_GetThreadPriority(ctx);
        if (priority == 32) {
            priority = cur ? cur - 1 : 0;
        } else if (priority == 33) {
            priority = cur < 31 ? cur + 1 : 31;
        } else if (priority == 34) {
            priority = cur;
        } else {
            priority = 31;
        }
    }
    e = OS_DisableInterrupts();
    task->busy = TRUE;
    task->priority = priority;
    task->func = func;
    task->done = done;
    if (ctx->list == NULL) {
        if (task == &ctx->idle) {
            data_02080f40 = NULL;
        }
        ctx->list = task;
        OS_WakeupThreadDirect(ctx);
    } else {
        UnkTask_0201a2e8 *p = ctx->list;
        if (task == &ctx->idle) {
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = task;
        data_02080f40 = NULL;
    } else if (p->priority > priority) {
            ctx->list = task;
            task->next = p;
        } else {
            UnkTask_0201a2e8 *q;
            while ((q = p->next) != NULL && q->priority <= priority) {
                p = q;
            }
            task->next = q;
            p->next = task;
        }
    }
    OS_RestoreInterrupts(e);
}
