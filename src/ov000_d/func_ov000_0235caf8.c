#include <nitro/types.h>

void SysSound_Release(void);
void func_02051628(s32 a);
BOOL func_020513a8(void);
void func_ov000_0235e31c(void);
void SceneChange_Unk_02055140(void);
void func_0204b528(void);
BOOL func_ov000_0235cbd8(void);
void SceneChange_Unk_0205507c(void);
void SceneChange_Unk_02055060(void);
void func_0204b624(void);
void SceneChange_Unk_02054e68(void);
void func_02051684(void (*f)(void));

#pragma thumb on

void func_ov000_0235caf8(s32 mode) {
    switch (mode) {
    case 0:
        SysSound_Release();
        func_02051628(1);
        if (func_020513a8()) {
            func_ov000_0235e31c();
        } else {
            SceneChange_Unk_02055140();
        }
        break;
    case 1:
        if (func_020513a8()) {
            func_0204b528();
        }
        SysSound_Release();
        if (func_ov000_0235cbd8()) {
            SceneChange_Unk_0205507c();
        } else {
            SceneChange_Unk_02055060();
        }
        break;
    case 2:
        SysSound_Release();
        if (func_020513a8()) {
            func_0204b528();
            func_0204b624();
        }
        func_02051684(SceneChange_Unk_02054e68);
        break;
    }
}

#pragma thumb reset
