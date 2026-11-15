#include <nitro/types.h>

extern s32 data_022c4560[];
void func_02063688(void);

#pragma thumb on
void func_ov001_022d6e6c(void) {
    data_022c4560[4] = 1;
    data_022c4560[6] |= 0x20;
    data_022c4560[6] &= ~0x10;
    func_02063688();
}
#pragma thumb reset
