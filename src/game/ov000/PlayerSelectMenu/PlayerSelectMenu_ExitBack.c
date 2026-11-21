#include <nitro/types.h>

void SceneChange_ToTitleMenu(void);
BOOL WirelessSession_IsDownloadPlay(void);
void Stage_FreeCommonFiles(void);
void SysSound_Release(void);
void WirelessSession_EndWhenAlone(void (*f)(void));
void SceneChange_ToTitleMenu_1_0(void);
void SceneChange_ToTitleMenu_1_1(void);

#pragma thumb on

void PlayerSelectMenu_ExitBack(s32 mode) {
    switch (mode) {
    case 0:
        SceneChange_ToTitleMenu();
        break;
    case 1:
        if (WirelessSession_IsDownloadPlay()) {
            Stage_FreeCommonFiles();
        }
        SysSound_Release();
        WirelessSession_EndWhenAlone(SceneChange_ToTitleMenu);
        break;
    case 2:
        SysSound_Release();
        SceneChange_ToTitleMenu();
        break;
    case 3:
        SceneChange_ToTitleMenu_1_0();
        break;
    case 4:
        SceneChange_ToTitleMenu_1_1();
        break;
    }
}

#pragma thumb reset
