#include <nitro/types.h>
#include <main_sdk2/wh.h>

extern u16 data_020779c0;
extern u16 data_020779c4;
extern u16 data_020779c8;
extern u16 data_020779cc;

void MBi_ParentCallback(UnkCallback_wh *arg);
void MBi_CheckWmErrcode(u16 id, s32 result);
s32 WM_SetIndCallback(void (*cb)(UnkCallback_wh *));
s32 WM_SetLifeTime(void (*cb)(UnkCallback_wh *), u16 a, u16 b, u16 c, u16 d);

void MBi_OnInitializeDone(void)
{
    MBi_CheckWmErrcode(0x80, WM_SetIndCallback(MBi_ParentCallback));
    MBi_CheckWmErrcode(0x1d, WM_SetLifeTime(MBi_ParentCallback, data_020779cc, data_020779c4, data_020779c0, data_020779c8));
}
