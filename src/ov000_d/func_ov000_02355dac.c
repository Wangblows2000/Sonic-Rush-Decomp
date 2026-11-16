#include <nitro/types.h>

void SceneChange_Unk_020550ec(void);
BOOL func_020513a8(void);
void func_0204b624(void);
void SysSound_Release(void);
void func_02051684(void (*f)(void));
void SceneChange_Unk_020550d0(void);
void SceneChange_Unk_020550b4(void);

#pragma thumb on

void func_ov000_02355dac(s32 mode) {
    switch (mode) {
    case 0:
        SceneChange_Unk_020550ec();
        break;
    case 1:
        if (func_020513a8()) {
            func_0204b624();
        }
        SysSound_Release();
        func_02051684(SceneChange_Unk_020550ec);
        break;
    case 2:
        SysSound_Release();
        SceneChange_Unk_020550ec();
        break;
    case 3:
        SceneChange_Unk_020550d0();
        break;
    case 4:
        SceneChange_Unk_020550b4();
        break;
    }
}

#pragma thumb reset
