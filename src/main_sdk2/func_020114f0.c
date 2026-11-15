#include <nitro/types.h>
#include <main_sdk2/wm.h>

s32 func_020114f0(u16 port, void *cb, void *arg)
{
    u32 e = OS_DisableInterrupts();
    s32 r = func_02010b54();
    u8 *work;
    if (r != 0) {
        OS_RestoreInterrupts(e);
        return r;
    }
    work = (u8 *)func_02010b70();
    ((void **)(work + 0xc0))[port] = cb;
    ((void **)(work + 0x100))[port] = arg;
    OS_RestoreInterrupts(e);
    return 0;
}
