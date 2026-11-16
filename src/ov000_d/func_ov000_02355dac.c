#include <nitro/types.h>

void SceneChange_ToTitleMenu(void);
BOOL func_020513a8(void);
void Stage_FreeCommonFiles(void);
void SysSound_Release(void);
void func_02051684(void (*f)(void));
void SceneChange_ToTitleMenu_1_0(void);
void SceneChange_ToTitleMenu_1_1(void);

#pragma thumb on

void func_ov000_02355dac(s32 mode) {
    switch (mode) {
    case 0:
        SceneChange_ToTitleMenu();
        break;
    case 1:
        if (func_020513a8()) {
            Stage_FreeCommonFiles();
        }
        SysSound_Release();
        func_02051684(SceneChange_ToTitleMenu);
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
