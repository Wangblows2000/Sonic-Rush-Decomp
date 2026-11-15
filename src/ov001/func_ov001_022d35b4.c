#include <nitro/types.h>

extern u16 data_0208922c[];
extern u16 data_02087d04[];

#pragma thumb on
void func_ov001_022d35b4(void) {
    data_0208922c[2] += 2;
    data_02087d04[2] += 2;
}
#pragma thumb reset
