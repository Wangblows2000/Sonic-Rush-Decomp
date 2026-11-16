#include <nitro/types.h>
#include <main_sdk2/osthread.h>

s32 func_02006094(void);
void func_0200602c(UnkOSThread *t);
void func_020060c4(UnkOSThread *t, void (*func)(void *), u32 sp);
void func_02005d00(void);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

void func_02005d5c(UnkOSThread *thread, void (*func)(void *), void *arg, u32 stack, u32 stackSize, u32 prio)
{
    u32 e = OS_DisableInterrupts();
    s32 idx = func_02006094();
    if (idx < 0) {
        OS_RestoreInterrupts(e);
        return;
    }
    thread->priority = prio;
    thread->id = idx;
    thread->state = 0;
    thread->profiler = NULL;
    data_0207d0a4[idx] = thread;
    func_0200602c(thread);
    thread->stackBottom = stack;
    thread->stackTop = stack - stackSize;
    thread->stackWarn = 0;
    *(u32 *)(thread->stackBottom - 4) = 0xfddb597d;
    *(u32 *)thread->stackTop = 0x7bf9dd5b;
    thread->joinQueue = 0;
    func_020060c4(thread, func, stack - 4);
    thread->r[0] = (u32)arg;
    thread->lr = (u32)func_02005d00;
    {
        vu32 z = 0;
        MIi_CpuClear32(z, (void *)(stack - stackSize + 4), stackSize - 8);
    }
    thread->mutex = NULL;
    thread->mutexHead = NULL;
    thread->mutexTail = NULL;
    OS_RestoreInterrupts(e);
}
