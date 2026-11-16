#include <nitro/types.h>

extern s32 data_ov001_0231f040;
void WirelessMgr_StopWfs(s32 a);

#pragma thumb on
void func_ov001_022c87b4(void) {
    WirelessMgr_StopWfs(0);
    data_ov001_0231f040 = 0;
}
#pragma thumb reset
