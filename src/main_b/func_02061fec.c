#include <nitro/types.h>

#pragma thumb on

void func_02061f08(s32 a, s32 b, s32 c, s32 d, void *e);
void func_02061fec(void) { func_02061f08(0, 0, 10, 10, (void *)0x6002000); }

#pragma thumb reset
