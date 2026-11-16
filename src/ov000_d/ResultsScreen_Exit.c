#include <nitro/types.h>

void SysSound_Release(void);
void Wireless_EnableErrorCallback(s32 a);
BOOL func_020513a8(void);
void ResultsScreen_CreateRematchSync(void);
void SceneChange_ToVsStage(void);
void Stage_FreeZoneFiles(void);
BOOL func_ov000_0235cbd8(void);
void SceneChange_ToPlayerSelectMenuMode1(void);
void SceneChange_ToPlayerSelectMenuMode2(void);
void Stage_FreeCommonFiles(void);
void SceneChange_ToTitleScreen(void);
void WirelessSession_EndWhenAlone(void (*f)(void));

#pragma thumb on

void ResultsScreen_Exit(s32 mode) {
    switch (mode) {
    case 0:
        SysSound_Release();
        Wireless_EnableErrorCallback(1);
        if (func_020513a8()) {
            ResultsScreen_CreateRematchSync();
        } else {
            SceneChange_ToVsStage();
        }
        break;
    case 1:
        if (func_020513a8()) {
            Stage_FreeZoneFiles();
        }
        SysSound_Release();
        if (func_ov000_0235cbd8()) {
            SceneChange_ToPlayerSelectMenuMode1();
        } else {
            SceneChange_ToPlayerSelectMenuMode2();
        }
        break;
    case 2:
        SysSound_Release();
        if (func_020513a8()) {
            Stage_FreeZoneFiles();
            Stage_FreeCommonFiles();
        }
        WirelessSession_EndWhenAlone(SceneChange_ToTitleScreen);
        break;
    }
}

#pragma thumb reset
