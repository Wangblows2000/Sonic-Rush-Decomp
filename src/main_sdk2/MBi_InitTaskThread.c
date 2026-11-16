#include <nitro/types.h>
#include <main_sdk2/task.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void MBi_InitTaskInfo(UnkTask_0201a2e8 *task);
void MBi_TaskThread(UnkTaskCtx_0201a2e8 *ctx);
void OS_CreateThread(void *thread, void (*func)(UnkTaskCtx_0201a2e8 *), void *arg, void *stack, u32 stackSize, u32 prio);
void OS_WakeupThreadDirect(void *thread);

void MBi_InitTaskThread(UnkTaskCtx_0201a2e8 *ctx, u32 size)
{
    u32 e = OS_DisableInterrupts();
    if (data_02080f40 == NULL) {
        u32 stackSize;
        data_02080f40 = ctx;
        MBi_InitTaskInfo(&ctx->idle);
        ctx->list = NULL;
        stackSize = (size - sizeof(UnkTaskCtx_0201a2e8)) & ~3;
        OS_CreateThread(ctx, MBi_TaskThread, ctx, (u8 *)ctx + sizeof(UnkTaskCtx_0201a2e8) + stackSize, stackSize, 0);
        OS_WakeupThreadDirect(ctx);
    }
    OS_RestoreInterrupts(e);
}
