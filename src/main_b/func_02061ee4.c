#include <nitro/types.h>

#pragma thumb on

void func_02061d68(s32 a, s32 b, s32 c, s32 d, void *e, s32 f);
void func_02061ee4(void) { func_02061d68(0, 0, 10, 10, (void *)0x6002000, 1); }

#pragma thumb reset
