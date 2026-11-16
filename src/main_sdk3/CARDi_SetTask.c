#include <nitro/types.h>

typedef struct {
    u8 pad0[0x30];
    void *task_func;   /* 0x30 */
    u32 flag;          /* 0x34 */
    u32 priority;      /* 0x38 */
    u8 thread[0xd0 - 0x3c];   /* 0x3c */
    void *cur_th;      /* 0xd0 */
} UnkCardCommon_0200f5ac;

typedef struct {
    u8 pad0[8];
    void *current;
} UnkThreadInfo_0200f5ac;

extern UnkCardCommon_0200f5ac data_0207f2c0;
extern UnkThreadInfo_0200f5ac data_0207d090;

extern u32 OS_GetThreadPriority(void *thread);
extern BOOL OS_SetThreadPriority(void *thread, u32 prio);
extern void OS_WakeupThreadDirect(void *thread);

void CARDi_SetTask(void *task)
{
    UnkCardCommon_0200f5ac *const p = &data_0207f2c0;
    u32 prio = p->priority;
    switch (prio) {
    case 0x20:
        prio = OS_GetThreadPriority(data_0207d090.current);
        if (prio != 0) {
            prio--;
        }
        break;
    case 0x21:
        prio = OS_GetThreadPriority(data_0207d090.current);
        if (prio < 31) {
            prio++;
        }
        break;
    }
    OS_SetThreadPriority(p->thread, prio);
    p->cur_th = p->thread;
    p->task_func = task;
    p->flag |= 8;
    OS_WakeupThreadDirect(p->thread);
}
