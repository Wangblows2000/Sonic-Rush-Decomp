#include <nitro/types.h>

extern s32 data_022c4560[];
void SceneChange_Unk_02054e14(void);

#pragma thumb on
void func_ov001_022d6e4c(void) {
    data_022c4560[4] = 1;
    data_022c4560[6] |= 0x20;
    data_022c4560[6] |= 0x10;
    SceneChange_Unk_02054e14();
}
#pragma thumb reset
