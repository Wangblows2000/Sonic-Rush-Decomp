#include <nitro/types.h>

typedef struct {
    s32 result;
} UnkCardCmd_0200f9a0;

typedef struct {
    UnkCardCmd_0200f9a0 *cmd;   /* 0x00 */
    u8 pad4[0x14];
    u32 src;          /* 0x18 */
    u32 dst;          /* 0x1c */
    u32 len;          /* 0x20 */
    u32 dma;          /* 0x24 */
    void *callback;   /* 0x28 */
    void *arg;        /* 0x2c */
    u8 pad30[4];
    u32 flag;         /* 0x34 */
    u8 pad38[0xd0 - 0x38];
    u32 priority;     /* 0xd0 */
    u8 queue[8];      /* 0xd4 */
} UnkCardCommon_0200f9a0;

typedef struct {
    u8 pad0[8];
    u32 f8;
} UnkThreadInfo_0200f9a0;

extern UnkCardCommon_0200f9a0 data_0207f2c0;
extern UnkThreadInfo_0200f9a0 data_0207d090;

extern u32 OS_DisableInterrupts(void);
extern u32 OS_RestoreInterrupts(u32 state);
extern void func_02005c48(void *queue);
extern void func_0200f5ac(void (*fn)(UnkCardCommon_0200f9a0 *));
extern void func_0200fb40(UnkCardCommon_0200f9a0 *p);

BOOL func_0200f9a0(u32 dst, u32 src, u32 len, void *callback, void *arg, BOOL is_async)
{
    UnkCardCommon_0200f9a0 *const p = &data_0207f2c0;
    u32 state = OS_DisableInterrupts();
    while (p->flag & 4) {
        func_02005c48(p->queue);
    }
    p->flag |= 4;
    OS_RestoreInterrupts(state);
    p->src = src;
    p->dst = dst;
    p->len = len;
    p->callback = callback;
    p->arg = arg;
    if (is_async) {
        func_0200f5ac(func_0200fb40);
        return TRUE;
    } else {
        data_0207f2c0.priority = data_0207d090.f8;
        func_0200fb40(p);
        return p->cmd->result == 0;
    }
}
