#include <nitro/types.h>

typedef struct {
    s32 result;
} UnkCardCmd_020102d8;

typedef void (*UnkCardCallback_020102d8)(void *arg);

typedef struct {
    UnkCardCmd_020102d8 *cmd;   /* 0x00 */
    u8 pad4[0x24];
    UnkCardCallback_020102d8 callback;   /* 0x28 */
    void *arg;        /* 0x2c */
    u8 pad30[4];
    u32 flag;         /* 0x34 */
    u32 priority;     /* 0x38 */
    u8 thread[0xd4 - 0x3c];   /* 0x3c */
    u8 queue[8];      /* 0xd4 */
} UnkCardCommon_020102d8;

extern UnkCardCommon_020102d8 data_0207f2c0;
extern u32 OS_DisableInterrupts(void);
extern u32 OS_RestoreInterrupts(u32 state);
extern void func_02005bd4(void *queue);
extern void func_02005ba0(void *thread);

void func_020102d8(void)
{
    UnkCardCommon_020102d8 *const p = &data_0207f2c0;
    UnkCardCallback_020102d8 callback = p->callback;
    void *arg = p->arg;
    u32 state;
    p->cmd->result = 0;
    state = OS_DisableInterrupts();
    p->flag &= ~0xc;
    func_02005bd4(p->queue);
    if (p->flag & 0x10) {
        func_02005ba0(p->thread);
    }
    OS_RestoreInterrupts(state);
    if (callback) {
        callback(arg);
    }
}
