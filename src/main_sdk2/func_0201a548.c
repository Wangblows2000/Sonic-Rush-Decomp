#include <nitro/types.h>
#include <main_sdk2/task.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void func_020059e0(void *thread, u32 prio);
void func_02005c48(void *queue);
u32 OS_GetThreadPriority(void *thread);
void func_02005d00(void);

void func_0201a548(UnkTaskCtx_0201a2e8 *ctx)
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
            func_020059e0(ctx, 0);
            func_02005c48(NULL);
        }
        task = ctx->list;
        ctx->list = ctx->list->next;
        func_020059e0(ctx, task->priority);
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
            func_020059e0(ctx, prio);
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
    func_02005d00();
}
