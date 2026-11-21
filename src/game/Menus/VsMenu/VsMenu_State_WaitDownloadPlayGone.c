#include <nitro/types.h>

extern s32 gVsMenuCancelled;
extern void *gTaskCurrent;
BOOL WirelessLobbyDownloadPlay_IsGone(void);
void Task_Destroy(void *p);

#pragma thumb on
void VsMenu_State_WaitDownloadPlayGone(void) {
    if (WirelessLobbyDownloadPlay_IsGone()) {
        gVsMenuCancelled = 0;
        Task_Destroy(gTaskCurrent);
    }
}
#pragma thumb reset
