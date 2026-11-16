#include <nitro/types.h>
#include <main_sdk2/osthread.h>

void func_020065d8(UnkOSThread *t);
void func_02005fe4(UnkOSThread *t);
void func_02005bd4(void *queue);
void func_02005a94(void);
void OS_Terminate(void);

void func_02005d00(void)
{
    UnkOSThread *cur;
    OS_DisableInterrupts();
    cur = data_0207d084->current;
    func_020065d8(cur);
    func_02005fe4(cur);
    data_0207d0a4[cur->id] = NULL;
    cur->state = 2;
    func_02005bd4(&cur->joinQueue);
    func_02005a94();
    OS_Terminate();
}
