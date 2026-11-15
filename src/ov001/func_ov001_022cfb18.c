#include <nitro/types.h>

extern s32 data_02087ccc;
extern u32 data_02087cb8;
void func_ov001_022cf93c(void);
void func_ov001_022cfac8(void);

#pragma thumb on
void func_ov001_022cfb18(void) {
    func_ov001_022cf93c();
    func_ov001_022cfac8();
    data_02087ccc = 0;
    data_02087cb8 &= ~0x10;
}
#pragma thumb reset
