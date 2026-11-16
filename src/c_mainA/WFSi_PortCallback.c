#include <nitro/types.h>

typedef struct {
    u8 pad0[0x8418];
    u32 unk8418;
} UnkData_0203d038;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkc;
    u8 pad10[0x430];
    UnkData_0203d038 data;
} UnkCtx_0203d038;

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    u32 unk8;
    u32 unkc;
    u16 unk10;
    u16 unk12;
} UnkArg_0203d038;

extern UnkCtx_0203d038 data_022b8be0;

void WFSi_SetMPData(void);
void WBT_MpParentRecvHook(u32 a, u16 b, u16 aid);
void WBT_MpChildRecvHook(u32 a, u16 b);

void WFSi_PortCallback(UnkArg_0203d038 *arg)
{
    u16 b;
    u32 a;
    u16 aid;
    UnkCtx_0203d038 *c = &data_022b8be0;
    UnkData_0203d038 *d = (UnkData_0203d038 *)((u32)c + 0x440);

    if (c->unkc == 0)
        return;
    switch (arg->unk4) {
    case 7:
        if (c->unk0 == 0)
            return;
        if (c->unk8 != 0)
            return;
        WFSi_SetMPData();
        break;
    case 9:
        if (c->unk0 == 0)
            return;
        d->unk8418 &= ~(1 << arg->unk12);
        break;
    case 0x15:
        a = arg->unkc;
        b = arg->unk10;
        if (c->unk0 != 0) {
            aid = arg->unk12;
            d->unk8418 |= 1 << aid;
            WBT_MpParentRecvHook(a, b, aid);
        } else {
            WBT_MpChildRecvHook(a, b);
        }
        break;
    }
}
