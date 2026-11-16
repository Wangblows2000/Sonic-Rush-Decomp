#include <nitro/types.h>

typedef struct {
    s32 result;
} UnkCardCmd_0200f63c;

typedef struct {
    UnkCardCmd_0200f63c *cmd;   /* 0x00 */
    u8 pad4[0x14];
    u32 src;          /* 0x18 */
    u32 dst;          /* 0x1c */
    u32 len;          /* 0x20 */
    u32 dma;          /* 0x24 */
    void *callback;   /* 0x28 */
    void *arg;        /* 0x2c */
    void (*task_func)(void *p);   /* 0x30 */
    u32 flag;         /* 0x34 */
    u32 priority;     /* 0x38 */
    u8 thread[0xd0 - 0x3c];   /* 0x3c */
    void *cur_th;     /* 0xd0 */
    u8 queue[8];      /* 0xd4 */
} UnkCardCommon_0200f63c;

typedef struct {
    u8 pad0[8];
    u32 f8;
} UnkThreadInfo_0200f63c;

extern UnkCardCommon_0200f63c data_0207f2c0;
extern UnkThreadInfo_0200f63c data_0207d090;

extern u32 OS_DisableInterrupts(void);
extern u32 OS_RestoreInterrupts(u32 state);
extern void OS_SleepThread(void *queue);
extern void CARDi_SetTask(void (*fn)(UnkCardCommon_0200f63c *));
extern void func_0200fb40(UnkCardCommon_0200f63c *p);

void CARDi_TaskThread(void)
{
    UnkCardCommon_0200f63c *const p = &data_0207f2c0;
    for (;;) {
        u32 state = OS_DisableInterrupts();
        for (;;) {
            if (p->flag & 8) {
                break;
            }
            p->cur_th = p->thread;
            OS_SleepThread(NULL);
        }
        OS_RestoreInterrupts(state);
        p->task_func(p);
    }
}
