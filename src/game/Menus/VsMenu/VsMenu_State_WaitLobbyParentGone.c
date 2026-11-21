#include <nitro/types.h>

extern s32 gVsMenuCancelled;
extern void *gTaskCurrent;
BOOL WirelessLobbyParent_IsGone(void);
void Task_Destroy(void *p);

#pragma thumb on
void VsMenu_State_WaitLobbyParentGone(void) {
    if (WirelessLobbyParent_IsGone()) {
        gVsMenuCancelled = 0;
        Task_Destroy(gTaskCurrent);
    }
}
#pragma thumb reset
