#include <nitro/types.h>

#pragma thumb on

void func_02040928(s32 *p) { p[0] = 0x100000; p[1] = 0x100000; }

#pragma thumb reset
