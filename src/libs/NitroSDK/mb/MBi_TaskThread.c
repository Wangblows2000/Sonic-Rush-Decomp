#include <nitro/types.h>
#include <NitroSDK/task.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void OS_SetThreadPriority(void *thread, u32 prio);
void OS_SleepThread(void *queue);
u32 OS_GetThreadPriority(void *thread);
void OS_ExitThread(void);

void MBi_TaskThread(UnkTaskCtx_0201a2e8 *ctx)
{
    u32 e;
    UnkTask_0201a2e8 *task;
    u32 e2;
    void (*done)(UnkTask_0201a2e8 *);
    u32 cur;
    u32 prio;

    for (;;) {
        e = OS_DisableInterrupts();
        while (ctx->list == NULL) {
            OS_SetThreadPriority(ctx, 0);
            OS_SleepThread(NULL);
        }
        task = ctx->list;
        ctx->list = ctx->list->next;
        OS_SetThreadPriority(ctx, task->priority);
        OS_RestoreInterrupts(e);
        if (task->func != NULL) {
            task->func(task);
        }
        e2 = OS_DisableInterrupts();
        done = task->done;
        cur = OS_GetThreadPriority(ctx);
        if (ctx->list == NULL) {
            prio = 0;
        } else if (cur < ctx->list->priority) {
            prio = ctx->list->priority;
        } else {
            prio = cur;
        }
        if (prio != cur) {
            OS_SetThreadPriority(ctx, prio);
        }
        task->next = NULL;
        task->busy = FALSE;
        if (done != NULL) {
            done(task);
        }
        if (task == &ctx->idle) {
            break;
        }
        OS_RestoreInterrupts(e2);
    }
    OS_ExitThread();
}
