#include <nitro/types.h>
#include <main_sdk2/wh.h>

extern u16 data_02080f10;
extern u16 *data_020779d0;

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MI_CpuCopy8(const void *src, void *dest, u32 size);
u16 func_02010f7c(void);
u16 func_02018e5c(void);

s32 func_02018c64(void *buf, UnkInitParam_wh *param, u32 ggid, u32 tgid, u32 dma)
{
    UnkWork_wh *work;
    UnkStruct_02080f1c *sub;
    u32 e;
    s32 i;
    u16 *dst;

    if (data_02080f1c != NULL && data_02080f1c->unk1316 != 0) {
        return 2;
    }
    work = (UnkWork_wh *)(((u32)buf + 0x1f) & ~0x1f);
    sub = (UnkStruct_02080f1c *)(((u32)work + 0x1e1f) & ~0x1f);
    if (tgid == 0x10000) {
        tgid = func_02010f7c();
    }
    e = OS_DisableInterrupts();
    data_02080f10 = dma;
    data_02080f18 = work;
    data_02080f1c = sub;
    {
        vu32 fill = 0;
        MIi_CpuClear32(fill, work, 0x1e00);
    }
    {
        vu16 fill = 0;
        MIi_CpuClear16(fill, sub, 0x1340);
    }
    dst = work->unk530;
    for (i = 0; i < param->count; i++) {
        *dst++ = param->data[i];
    }
    dst = work->unk538;
    for (i = 0; i < 16; i++) {
        if (*data_020779d0 == 0) {
            break;
        }
        *dst++ = *data_020779d0++;
    }
    MI_CpuCopy8(param, &sub->initParam, sizeof(UnkInitParam_wh));
    if (param->count < 10) {
        sub->initParam.data[param->count] = 0;
    }
    work->unk500 = 0x100;
    work->unk502 = 8;
    work->sendBufSize = 0;
    work->recvBufSize = 0;
    work->unk52c = 1;
    work->recvBuf = (u8 *)sub + 0x400;
    work->parentParam.entryFlag = 1;
    work->parentParam.multiBootFlag = 0;
    work->parentParam.CS_Flag = 1;
    work->parentParam.KS_Flag = 0;
    work->parentParam.ggid = ggid;
    work->parentParam.tgid = tgid;
    work->parentParam.beaconPeriod = func_02018e5c() + 200;
    work->parentParam.maxEntry = 15;
    work->unk50c = 0;
    work->unk50d = 0;
    sub->unk1316 = 1;
    sub->unk131c = 0;
    OS_RestoreInterrupts(e);
    return 0;
}
