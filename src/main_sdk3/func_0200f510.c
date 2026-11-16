#include <nitro/types.h>

typedef struct {
    s32 result;
} UnkCardCmd_0200f510;

typedef struct {
    UnkCardCmd_0200f510 *cmd;   /* 0x00 */
    u8 pad4[4];
    vs32 lock_owner;             /* 0x08 */
    s32 lock_ref;               /* 0x0c */
    u8 lock_queue[4];           /* 0x10 */
    u32 lock_target;            /* 0x14 */
} UnkCardCommon_0200f510;

extern UnkCardCommon_0200f510 data_0207f2c0;

extern u32 OS_DisableInterrupts(void);
extern u32 OS_RestoreInterrupts(u32 state);
extern void func_02005c48(void *queue);
extern void OS_Terminate(void);

void func_0200f510(s32 owner, u32 target)
{
    UnkCardCommon_0200f510 *const p = &data_0207f2c0;
    u32 state = OS_DisableInterrupts();
    if (p->lock_owner == owner) {
        if (p->lock_target != target) {
            OS_Terminate();
        }
    } else {
        while (p->lock_owner != -3) {
            func_02005c48(p->lock_queue);
        }
        p->lock_owner = owner;
        p->lock_target = target;
    }
    p->lock_ref++;
    p->cmd->result = 0;
    OS_RestoreInterrupts(state);
}
