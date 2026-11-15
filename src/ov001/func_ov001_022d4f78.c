#include <nitro/types.h>

extern s32 data_02087ccc;
extern u32 data_02087cb8;
void func_ov001_022d4f30(void);

#pragma thumb on
void func_ov001_022d4f78(void) {
    func_ov001_022d4f30();
    data_02087ccc = 0;
    data_02087cb8 &= ~0x10;
}
#pragma thumb reset
