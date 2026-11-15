#include <nitro/types.h>

#pragma thumb on

void func_02060e54(s32 i, u32 v);
u32 func_02060e70(s32 i);
void func_020607c8(void *s);
void func_02060ba4(void);
void func_02060bd8(void (**s)(void)) { func_02060e54(2, 0x10); func_02060e54(1, 0x10); if (!(func_02060e70(0) & 2)) func_02060e54(3, 0x20000000); func_020607c8(s); *s = func_02060ba4; }

#pragma thumb reset
