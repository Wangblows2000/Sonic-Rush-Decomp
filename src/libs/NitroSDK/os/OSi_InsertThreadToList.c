#include <nitro/types.h>
#include <NitroSDK/osthread.h>

void OSi_InsertThreadToList(UnkOSThread *thread)
{
    UnkOSThread *t = data_0207d090.list;
    UnkOSThread *pre = NULL;
    while (t != NULL && t->priority < thread->priority) {
        pre = t;
        t = t->next;
    }
    if (pre == NULL) {
        thread->next = data_0207d090.list;
        data_0207d090.list = thread;
    } else {
        thread->next = pre->next;
        pre->next = thread;
    }
}
