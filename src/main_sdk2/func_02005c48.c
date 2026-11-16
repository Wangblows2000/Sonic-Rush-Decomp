#include <nitro/types.h>
#include <main_sdk2/osthread.h>

void func_02005a94(void);

void func_02005c48(u16 *queue)
{
    u32 e = OS_DisableInterrupts();
    UnkOSThread *cur = data_0207d084->current;
    if (queue != NULL) {
        *queue |= (u16)(1 << cur->id);
    }
    cur->state = 0;
    func_02005a94();
    OS_RestoreInterrupts(e);
}
