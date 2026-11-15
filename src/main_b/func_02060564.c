#include <nitro/types.h>

#pragma thumb on

extern u16 data_022503e0;
u32 func_02060e70(s32 i);
void func_02060e54(s32 i, u32 v);
BOOL func_02058314(void);
void func_02060564(void) { if (func_02060e70(0) & 0x100) { if ((data_022503e0 & 8) || func_02058314()) func_02060e54(0, 0x200); } }

#pragma thumb reset
