#include <nitro/types.h>

typedef struct {
    s32 result;
    u8 pad4[8];
    u32 src;     /* 0x0c */
    u32 dst;     /* 0x10 */
    u32 len;     /* 0x14 */
} UnkCardCmd_0200f8b8;

typedef struct {
    UnkCardCmd_0200f8b8 *cmd;   /* 0x00 */
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
    u8 pad_dc[4];
    u8 buf[0x100];    /* 0xe0 */
} UnkCardCommon_0200f8b8;

typedef struct {
    u8 pad0[8];
    u32 f8;
} UnkThreadInfo_0200f8b8;

extern UnkCardCommon_0200f8b8 data_0207f2c0;
extern UnkThreadInfo_0200f8b8 data_0207d090;

extern u32 OS_DisableInterrupts(void);
extern u32 OS_RestoreInterrupts(u32 state);
extern void func_02005c48(void *queue);
extern void func_0200f5ac(void (*fn)(UnkCardCommon_0200f8b8 *));
extern void func_0200f31c(void);
extern void func_0200f6e0(u32 id);
extern BOOL func_020103f0(UnkCardCommon_0200f8b8 *p, u32 req, u32 retry);
extern void func_0200fcb8(UnkCardCommon_0200f8b8 *p);

BOOL func_0200f8b8(u32 id)
{
    UnkCardCommon_0200f8b8 *const p = &data_0207f2c0;
    u32 state;
    func_0200f31c();
    state = OS_DisableInterrupts();
    while (p->flag & 4) {
        func_02005c48(p->queue);
    }
    p->flag |= 4;
    OS_RestoreInterrupts(state);
    func_0200f6e0(id);
    p->callback = NULL;
    p->arg = NULL;
    data_0207f2c0.priority = data_0207d090.f8;
    func_020103f0(p, 2, 1);
    p->cmd->src = 0;
    p->cmd->dst = (u32)p->buf;
    p->cmd->len = 1;
    func_020103f0(p, 6, 1);
    func_0200fcb8(p);
    return p->cmd->result == 0;
}
