#include <nitro/types.h>
#include <main_sdk2/osthread.h>

extern u32 data_0207d080;
extern void (*data_0207d08c)(UnkOSThread *from, UnkOSThread *to);

u32 OS_GetProcMode(void);
UnkOSThread *func_02005b74(void);
BOOL OS_SaveContext(UnkOSThread *t);
void OS_LoadContext(UnkOSThread *t);

void func_02005a94(void)
{
    UnkOSThreadInfo2 *info = &data_0207d090;
    UnkOSThread *cur;
    UnkOSThread *next;
    if (data_0207d080 != 0) {
        return;
    }
    if (info->irqDepth > 0 || OS_GetProcMode() == 0x12) {
        info->isNeedRescheduling = TRUE;
        return;
    }
    cur = data_0207d084->current;
    next = func_02005b74();
    if (cur == next || next == NULL) {
        return;
    }
    if (OS_SaveContext(cur)) {
        return;
    }
    if (data_0207d08c != NULL) {
        data_0207d08c(cur, next);
    }
    if (info->switchCallback != NULL) {
        info->switchCallback(cur, next);
    }
    data_0207d090.current = next;
    OS_LoadContext(next);
}
