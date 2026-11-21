#include <nitro/types.h>

typedef struct {
    s32 result;
} UnkCardCmd_0200f46c;

typedef struct {
    UnkCardCmd_0200f46c *cmd;   /* 0x00 */
    u8 pad4[4];
    vs32 lock_owner;             /* 0x08 */
    vs32 lock_ref;              /* 0x0c */
    u8 lock_queue[4];           /* 0x10 */
    u32 lock_target;            /* 0x14 */
} UnkCardCommon_0200f46c;

extern UnkCardCommon_0200f46c data_0207f2c0;

extern u32 OS_DisableInterrupts(void);
extern u32 OS_RestoreInterrupts(u32 state);
extern void OS_SleepThread(void *queue);
extern void OS_Terminate(void);
extern void OS_WakeupThread(void *queue);

void CARDi_UnlockResource(s32 owner, u32 target)
{
    UnkCardCommon_0200f46c *p = &data_0207f2c0;
    u32 state = OS_DisableInterrupts();
    if (p->lock_owner != owner || p->lock_ref == 0) {
        OS_Terminate();
    } else {
        if (p->lock_target != target) {
            OS_Terminate();
        }
        if (--p->lock_ref == 0) {
            p->lock_owner = -3;
            p->lock_target = 0;
            OS_WakeupThread(p->lock_queue);
        }
    }
    p->cmd->result = 0;
    OS_RestoreInterrupts(state);
}
