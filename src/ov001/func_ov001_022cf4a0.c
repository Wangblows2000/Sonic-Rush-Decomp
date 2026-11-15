#include <nitro/types.h>

extern s32 data_02087ccc;
extern u32 data_02087cb8;
void func_ov001_022cf3b0(void);

#pragma thumb on
void func_ov001_022cf4a0(void) {
    func_ov001_022cf3b0();
    data_02087ccc = 0;
    data_02087cb8 &= ~0x10;
}
#pragma thumb reset
