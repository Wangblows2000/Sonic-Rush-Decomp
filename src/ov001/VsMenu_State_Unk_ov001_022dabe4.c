#include <nitro/types.h>

extern s32 gVsMenuCancelled;
extern void *gTaskCurrent;
BOOL WirelessLobbyChild_IsGone(void);
void Task_Destroy(void *p);

#pragma thumb on
void VsMenu_State_Unk_ov001_022dabe4(void) {
    if (WirelessLobbyChild_IsGone()) {
        gVsMenuCancelled = 0;
        Task_Destroy(gTaskCurrent);
    }
}
#pragma thumb reset
