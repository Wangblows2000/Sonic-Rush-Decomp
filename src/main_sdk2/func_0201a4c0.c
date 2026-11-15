#include <nitro/types.h>
#include <main_sdk2/task.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void func_0201a490(UnkTask_0201a2e8 *task);
void func_0201a548(UnkTaskCtx_0201a2e8 *ctx);
void func_02005d5c(void *thread, void (*func)(UnkTaskCtx_0201a2e8 *), void *arg, void *stack, u32 stackSize, u32 prio);
void func_02005ba0(void *thread);

void func_0201a4c0(UnkTaskCtx_0201a2e8 *ctx, u32 size)
{
    u32 e = OS_DisableInterrupts();
    if (data_02080f40 == NULL) {
        u32 stackSize;
        data_02080f40 = ctx;
        func_0201a490(&ctx->idle);
        ctx->list = NULL;
        stackSize = (size - sizeof(UnkTaskCtx_0201a2e8)) & ~3;
        func_02005d5c(ctx, func_0201a548, ctx, (u8 *)ctx + sizeof(UnkTaskCtx_0201a2e8) + stackSize, stackSize, 0);
        func_02005ba0(ctx);
    }
    OS_RestoreInterrupts(e);
}
