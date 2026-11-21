#include <nitro/types.h>

typedef struct {
    void *cmd;        /* 0x00 */
    u8 pad4[4];
    vs32 lockOwner;   /* 0x08 */
    vs32 lockRef;     /* 0x0c */
    u16 unk10;        /* 0x10 */
    u8 pad12[2];
    u32 lockTarget;   /* 0x14 */
    u8 pad18[0x20];
    u32 priority;     /* 0x38 */
    u8 thread[0xd4 - 0x3c]; /* 0x3c */
    u16 unkD4;        /* 0xd4 */
} UnkCardCommon_0200f358;

extern UnkCardCommon_0200f358 data_0207f2c0;
extern u8 data_0207f120[];
extern u8 data_0207f160[];
extern u8 data_0207f8a0[];

void MIi_CpuClearFast(u32 data, void *dest, u32 size);
void DC_FlushRange(void *p, u32 size);
u32 OS_GetProtectionRegion2(void);
void OS_SetProtectionRegion2(u32 a);
void MI_CpuCopy8(const void *src, void *dest, u32 size);
void OS_CreateThread(void *thread, void (*fn)(void *), void *arg, void *stack, u32 stackSize, u32 prio);
void OS_WakeupThreadDirect(void *thread);
void PXI_SetFifoRecvCallback(u32 tag, void (*fn)(u32, u32, BOOL));
void CARD_Enable(BOOL a);
void CARDi_TaskThread(void *arg);
void CARDi_OnFifoRecv(u32 tag, u32 data, BOOL err);

void CARDi_InitCommon(void)
{
    UnkCardCommon_0200f358 *p = &data_0207f2c0;

    p->lockOwner = -3;
    p->lockRef = 0;
    p->lockTarget = 0;
    p->cmd = data_0207f120;
    {
        vu32 z = 0;
        MIi_CpuClearFast(z, data_0207f120, 0x40);
    }
    DC_FlushRange(data_0207f120, 0x40);
    {
        u32 prev = OS_GetProtectionRegion2();
        OS_SetProtectionRegion2(0);
        MI_CpuCopy8((*(u16 *)0x027ffc40 == 2) ? (void *)0x023fe940 : (void *)0x027ffe00, data_0207f160, 0x160);
        OS_SetProtectionRegion2(prev);
    }
    p->unk10 = 0;
    p->unkD4 = 0;
    p->priority = 4;
    OS_CreateThread(p->thread, CARDi_TaskThread, NULL, data_0207f8a0, 0x400, p->priority);
    OS_WakeupThreadDirect(p->thread);
    PXI_SetFifoRecvCallback(11, CARDi_OnFifoRecv);
    if (*(u16 *)0x027ffc40 != 2) {
        CARD_Enable(TRUE);
    }
}
