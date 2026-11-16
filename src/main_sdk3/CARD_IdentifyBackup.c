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
extern void OS_SleepThread(void *queue);
extern void CARDi_SetTask(void (*fn)(UnkCardCommon_0200f8b8 *));
extern void CARD_CheckEnabled(void);
extern void CARDi_IdentifyBackupCore(u32 id);
extern BOOL CARDi_Request(UnkCardCommon_0200f8b8 *p, u32 req, u32 retry);
extern void CARDi_EndTask(UnkCardCommon_0200f8b8 *p);

BOOL CARD_IdentifyBackup(u32 id)
{
    UnkCardCommon_0200f8b8 *const p = &data_0207f2c0;
    u32 state;
    CARD_CheckEnabled();
    state = OS_DisableInterrupts();
    while (p->flag & 4) {
        OS_SleepThread(p->queue);
    }
    p->flag |= 4;
    OS_RestoreInterrupts(state);
    CARDi_IdentifyBackupCore(id);
    p->callback = NULL;
    p->arg = NULL;
    data_0207f2c0.priority = data_0207d090.f8;
    CARDi_Request(p, 2, 1);
    p->cmd->src = 0;
    p->cmd->dst = (u32)p->buf;
    p->cmd->len = 1;
    CARDi_Request(p, 6, 1);
    CARDi_EndTask(p);
    return p->cmd->result == 0;
}
