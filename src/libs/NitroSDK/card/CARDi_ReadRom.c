#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u32 src;          /* 0x18 */
    void *dst;        /* 0x1c */
    u32 len;          /* 0x20 */
    u32 dma;          /* 0x24 */
    void *callback;   /* 0x28 */
    void *arg;        /* 0x2c */
    u8 pad30[4];
    u32 flag;         /* 0x34 */
    u8 pad38[0xd0 - 0x38];
    u32 priority;     /* 0xd0 */
    u8 queue[8];      /* 0xd4 */
} UnkCardCommon_0200fdbc;

typedef struct {
    u8 pad0[8];
    u32 f8;
} UnkThreadInfo_0200fdbc;

extern UnkCardCommon_0200fdbc data_0207f2c0;
extern u32 data_0207f8a0;
extern u8 data_0207f8c0[];
extern UnkThreadInfo_0200fdbc data_0207d090;

extern void CARD_CheckEnabled(void);
extern u32 OS_DisableInterrupts(void);
extern u32 OS_RestoreInterrupts(u32 state);
extern void OS_SleepThread(void *queue);
extern void MI_StopDma(u32 dma);
extern BOOL CARDi_TryReadCardDma(void *p);
extern void CARD_WaitRomAsync(void);
extern void CARDi_SetTask(void (*fn)(UnkCardCommon_0200fdbc *));
extern void CARDi_ReadRomSyncCore(UnkCardCommon_0200fdbc *p);

void CARDi_ReadRom(u32 dma, u32 src, void *dst, u32 len, void *callback, void *arg, BOOL is_async)
{
    UnkCardCommon_0200fdbc *const p = &data_0207f2c0;
    u8 *rom = data_0207f8c0;
    u32 state;
    CARD_CheckEnabled();
    state = OS_DisableInterrupts();
    while (p->flag & 4) {
        OS_SleepThread(p->queue);
    }
    p->flag |= 4;
    OS_RestoreInterrupts(state);
    p->dma = dma;
    p->src = src + data_0207f8a0;
    p->dst = dst;
    p->len = len;
    p->callback = callback;
    p->arg = arg;
    if (dma <= 3) {
        MI_StopDma(dma);
    }
    if (CARDi_TryReadCardDma(rom)) {
        if (!is_async) {
            CARD_WaitRomAsync();
        }
    } else if (is_async) {
        CARDi_SetTask(CARDi_ReadRomSyncCore);
    } else {
        p->priority = data_0207d090.f8;
        CARDi_ReadRomSyncCore(p);
    }
}
