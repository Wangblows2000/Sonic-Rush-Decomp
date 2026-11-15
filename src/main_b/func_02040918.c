#include <nitro/types.h>

#pragma thumb on

void func_02040918(s32 *p) { p[0] = 0x1000; p[1] = 0x1000; p[2] = 0; }

#pragma thumb reset
