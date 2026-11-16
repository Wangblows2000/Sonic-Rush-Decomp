#include <nitro/types.h>

extern u16 data_022c4560[];
void SceneChange_Unk_02055124(void);

#pragma thumb on
void func_ov001_022d41c4(void) {
    data_022c4560[0xf]++;
    if (data_022c4560[0xf] >= 4) {
        data_022c4560[0xf] = 0;
    }
    SceneChange_Unk_02055124();
}
#pragma thumb reset
