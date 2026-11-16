#include <nitro/types.h>

extern u16 data_0207ad9c;
extern void *data_022c4638;
extern u32 data_02087cb8;
void func_0204331c(s32 a);

#pragma thumb on
void func_02050964(void *task) {
    func_0204331c(0);
    data_0207ad9c = 4;
    if (task == data_022c4638)
        data_022c4638 = NULL;
    data_02087cb8 &= ~0x200;
}
#pragma thumb reset
