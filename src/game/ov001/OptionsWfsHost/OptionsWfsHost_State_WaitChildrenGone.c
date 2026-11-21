#include <nitro/types.h>

extern void *gTaskCurrent;
extern s32 gOptionsWfsHostError;
u32 WH_GetBitmap(void);
void WirelessMgr_StopWfs(s32 a);
void Task_Destroy(void *p);

#pragma thumb on
void OptionsWfsHost_State_WaitChildrenGone(void) {
    if ((WH_GetBitmap() & ~1) == 0) {
        WirelessMgr_StopWfs(0);
        Task_Destroy(gTaskCurrent);
        gOptionsWfsHostError = 0;
    }
}
#pragma thumb reset
