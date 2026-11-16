#include <nitro/types.h>

typedef struct {
    s32 result;
} UnkCardCmd_020103f0;

typedef struct {
    UnkCardCmd_020103f0 *cmd; /* 0x00 */
    s32 command;              /* 0x04 */
    u8 pad8[0x2c];
    vu32 flag;                /* 0x34 */
} UnkCardCommon_020103f0;

BOOL PXI_IsCallbackReady(u32 fifotag, u32 proc);
void OS_SpinWait(u32 cycles);
void DC_FlushRange(void *p, u32 size);
void DC_WaitWriteBufferEmpty(void);
s32 func_0200a0d8(u32 fifotag, u32 data, BOOL err);
u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);
void func_02005c48(void *queue);

static inline void func_020103f0_SendPxi(u32 data)
{
    while (func_0200a0d8(11, data, TRUE) < 0) {
    }
}

BOOL func_020103f0(UnkCardCommon_020103f0 *p, s32 req_type, s32 retry_count)
{
    if ((p->flag & 2) == 0) {
        p->flag |= 2;
        while (!PXI_IsCallbackReady(11, 1)) {
            OS_SpinWait(100);
        }
        (void)func_020103f0(p, 0, 1);
    }
    DC_FlushRange(p->cmd, 0x40);
    DC_WaitWriteBufferEmpty();
    do {
        p->command = req_type;
        p->flag |= 0x20;
        func_020103f0_SendPxi((u32)req_type);
        switch (req_type) {
        case 0:
            func_020103f0_SendPxi((u32)p->cmd);
            break;
        }
        {
            u32 enabled = OS_DisableInterrupts();
            while ((p->flag & 0x20) != 0) {
                func_02005c48(NULL);
            }
            (void)OS_RestoreInterrupts(enabled);
        }
    } while ((p->cmd->result == 4) && (--retry_count > 0));
    return (p->cmd->result == 0);
}
