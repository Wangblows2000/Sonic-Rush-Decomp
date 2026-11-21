#include <nitro/types.h>
#include <NitroSDK/ctrdg.h>

extern u32 data_02080f58;
extern u32 data_02080f80[];
extern u8 data_02080f84[];

u32 OS_SetIrqMask(u32 mask);
void DC_InvalidateRange(void *p, u32 size);
void DC_FlushAll(void);
void MI_DmaCopy16(u32 dma, const void *src, void *dst, u32 size);
void MIi_CpuCopy32(const void *src, void *dst, u32 size);
void CTRDGi_SendtoPxi(u32 data);
void WaitByLoop(s32 count);

static inline u16 func_0201ad7c_SetIme(u16 v)
{
    u16 prev = *(vu16 *)0x04000208;
    *(vu16 *)0x04000208 = v;
    return prev;
}

void CTRDGi_InitModuleInfo(void)
{
    UnkCtrdgLock lock;
    UnkCtrdgRomCycle cycle;
    u32 mask;
    u16 ime;
    u32 phi;
    s32 i;
    UnkCtrdgModuleInfo *info;
    UnkCtrdgHeader *hdr;

    if (data_02080f58 != 0) {
        return;
    }
    data_02080f58 = 1;
    if (!(*(vu16 *)0x04000300 & 1)) {
        return;
    }
    mask = OS_SetIrqMask(0x40000);
    ime = func_0201ad7c_SetIme(1);
    CTRDGi_LockByProcessor(data_02080f54.lockId, &lock);
    phi = (*(vu16 *)0x04000204 & 0x8000) >> 15;
    CTRDGi_ChangeLatestAccessCycle(&cycle);
    *(vu16 *)0x04000204 &= ~0x8000;
    DC_InvalidateRange((u8 *)data_02080f80 + 0x80, 0x40);
    MI_DmaCopy16(1, (void *)0x08000080, (u8 *)data_02080f80 + 0x80, 0x40);
    *(vu16 *)0x04000204 = (*(vu16 *)0x04000204 & ~0x8000) | (phi << 15);
    CTRDGi_RestoreAccessCycle(&cycle);
    CTRDGi_UnlockByProcessor(data_02080f54.lockId, &lock);
    info = CTRDG_MODULE_INFO;
    hdr = (UnkCtrdgHeader *)data_02080f80;
    info->moduleID = hdr->moduleID;
    for (i = 0; i < 3; i++) {
        info->exLsiID[i] = hdr->exLsiID[i];
    }
    info->makerCode = hdr->makerCode;
    info->gameCode = hdr->gameCode;
    MIi_CpuCopy32((void *)0xffff0020, data_02080f84, 0x9c);
    DC_FlushAll();
    CTRDGi_SendtoPxi((((u32)data_02080f80 - 0x02000000) >> 5) << 6 | 1);
    while (*(vu16 *)&data_02080f54.unk0 != 1) {
        WaitByLoop(1);
    }
    func_0201ad7c_SetIme(ime);
    OS_SetIrqMask(mask);
}
