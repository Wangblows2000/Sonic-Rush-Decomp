#include <nitro/types.h>
#include <NitroSDK/wh.h>

extern void *data_02080f14;
extern u16 data_02080f10;

void MBi_SetMaxScanTime(u32 a);
s32 WM_Initialize(void *buf, void (*cb)(UnkCallback_wh *), u16 dma);
s32 WM_SetIndCallback(void (*cb)(UnkCallback_wh *));
void MBi_OnInitializeDone(void);

s32 MBi_StartCommon(void)
{
    s32 r;

    data_02080f18->unk528 = 0;
    data_02080f18->unk52a = 0;
    data_02080f18->unk526 = 0;
    data_02080f18->unk548 = 0;
    MBi_SetMaxScanTime(0xa);
    if (data_02080f1c->unk1320 == 0) {
        do {
            r = WM_Initialize(data_02080f14, data_02080f18->unk508, data_02080f10);
        } while (r == 4);
        if (r != 2) {
            return 8;
        }
        WM_SetIndCallback(data_02080f18->unk508);
        data_02080f18->unk50d = 1;
        return 0;
    }
    WM_SetIndCallback(data_02080f18->unk508);
    data_02080f18->unk50d = 1;
    MBi_OnInitializeDone();
    return 0;
}
