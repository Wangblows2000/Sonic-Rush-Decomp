#include <nitro/types.h>

#pragma thumb on

void func_02061d68(s32 a, s32 b, s32 c, s32 d, void *e, s32 f);
void func_02061eac(void) { func_02061d68(0, 0, 0x16, 10, (void *)0x6003000, 1); func_02061d68(3, 1, 0x10, 7, (void *)0x6003000, 0); }

#pragma thumb reset
