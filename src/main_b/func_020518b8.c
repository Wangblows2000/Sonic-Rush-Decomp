#include <nitro/types.h>

#pragma thumb on

extern void *data_022c4638;
extern void *data_022c4634;
extern u16 gWirelessSession[2];
void func_020518b8(void) { data_022c4638 = NULL; data_022c4634 = NULL; gWirelessSession[0] = 4; gWirelessSession[1] = 5; }

#pragma thumb reset
