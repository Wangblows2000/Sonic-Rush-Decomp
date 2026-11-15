#include <nitro/types.h>
#include <main_sdk2/wh.h>

extern void *data_02080f14;

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MI_CpuFill8(void *dest, u8 data, u32 size);
void func_02016b6c(u32 a);
void func_0201a9a0(u16 a);
void func_0201a980(void *a);
void func_02018004(void);
s32 func_02018a50(void);
u32 PXI_IsCallbackReady(u32 fifo, u32 proc);
void func_0201640c(u32 a, void *b);
void func_020197bc(UnkCallback_wh *arg);

s32 func_0201882c(u16 channel)
{
    u32 e;
    u32 saved;
    s32 i;
    s32 ret;

    e = OS_DisableInterrupts();
    data_02080f18->parentParam.channel = channel;
    data_02080f14 = (void *)(((u32)data_02080f1c + 0x7d1f) & ~0x1f);
    saved = (u32)data_02080f1c->unk14e4;
    {
        vu16 fill = 0;
        MIi_CpuClear16(fill, data_02080f1c->childInit, 0x69c0);
    }
    func_02016b6c(saved);
    data_02080f1c->unk1318 = data_02080f18->unk500 - 6;
    func_0201a9a0(data_02080f18->unk502);
    func_0201a980(data_02080f1c->unk1538);
    for (i = 0; i < 15; i++) {
        data_02080f1c->unk14e8[i] = 0;
        data_02080f1c->unk1526[i] = -1;
    }
    data_02080f1c->unk1524 = 0;
    {
        vu16 fill = 0;
        MIi_CpuClear16(fill, data_02080f1c->unk1788, sizeof(data_02080f1c->unk1788));
    }
    MI_CpuFill8(data_02080f1c->unk1754, 0, sizeof(data_02080f1c->unk1754));
    data_02080f18->unk524 = 1;
    data_02080f18->callback = func_0201640c;
    data_02080f18->unk508 = func_020197bc;
    data_02080f18->parentParam.parentMaxSize = data_02080f18->unk500;
    data_02080f18->sendBufSize = (data_02080f18->parentParam.parentMaxSize + 0x23) & ~0x1f;
    data_02080f18->parentParam.childMaxSize = data_02080f18->unk502;
    data_02080f18->recvBufSize = (((data_02080f18->parentParam.childMaxSize + 0xe) * 15 + 0x29) & ~0x1f) << 1;
    func_02018004();
    ret = func_02018a50();
    OS_RestoreInterrupts(e);
    data_02080f1c->unk74c8 = PXI_IsCallbackReady(0xf, 1);
    return ret;
}
