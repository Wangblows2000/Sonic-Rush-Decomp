#include <nitro/types.h>

#pragma thumb on

extern void *data_022c4638;
extern void *data_022c4634;
extern u16 data_0207ad9c[2];
void func_020518b8(void) { data_022c4638 = NULL; data_022c4634 = NULL; data_0207ad9c[0] = 4; data_0207ad9c[1] = 5; }

#pragma thumb reset
