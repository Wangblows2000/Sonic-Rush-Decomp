#include <nitro/types.h>

extern u16 data_0208922c[];
extern u16 data_02087d04[];

#pragma thumb on
void func_ov001_022e9dac(void) {
    data_0208922c[6] += 2;
    data_02087d04[6] += 2;
}
#pragma thumb reset
