#include <nitro/types.h>

extern u32 data_02087cb8;
extern u16 data_0208922c[];

#pragma thumb on
void func_ov001_022ea290(void) {
    data_02087cb8 &= ~0x400;
    data_0208922c[2] = 0;
    data_0208922c[3] = 0;
}
#pragma thumb reset
