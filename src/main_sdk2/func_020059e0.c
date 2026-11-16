#include <nitro/types.h>
#include <main_sdk2/osthread.h>

void func_0200602c(UnkOSThread *t);
void func_02005a94(void);

BOOL func_020059e0(UnkOSThread *thread, u32 prio)
{
    UnkOSThread *t = data_0207d090.list;
    UnkOSThread *pre = NULL;
    u32 e = OS_DisableInterrupts();
    while (t != NULL && t != thread) {
        pre = t;
        t = t->next;
    }
    if (t == NULL || t == &data_0207d0e4) {
        OS_RestoreInterrupts(e);
        return FALSE;
    }
    if (t->priority != prio) {
        if (pre == NULL) {
            data_0207d090.list = thread->next;
        } else {
            pre->next = thread->next;
        }
        thread->priority = prio;
        func_0200602c(thread);
        func_02005a94();
    }
    OS_RestoreInterrupts(e);
    return TRUE;
}
