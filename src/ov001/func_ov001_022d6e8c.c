#include <nitro/types.h>

extern s32 data_022c4560[];
void func_020636ec(void);

#pragma thumb on
void func_ov001_022d6e8c(void) {
    data_022c4560[4] = 1;
    data_022c4560[6] |= 0x20;
    data_022c4560[6] &= ~0x10;
    func_020636ec();
}
#pragma thumb reset
