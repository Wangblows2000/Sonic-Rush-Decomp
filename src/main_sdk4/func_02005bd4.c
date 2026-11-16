#include <nitro/types.h>
#include <main_sdk2/osthread.h>

void func_02005a94(void);
u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);

static inline void WakeupMask(u32 mask)
{
    UnkOSThread *t = data_0207d090.list;
    while (t != NULL) {
        if (mask & (1 << t->id)) {
            t->state = 1;
        }
        t = t->next;
    }
}

void func_02005bd4(u16 *queue)
{
    u32 e = OS_DisableInterrupts();
    if (*queue != 0) {
        WakeupMask(*queue);
        *queue = 0;
        func_02005a94();
    }
    OS_RestoreInterrupts(e);
}
