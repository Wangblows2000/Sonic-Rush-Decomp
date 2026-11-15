#include <nitro/types.h>

extern u16 data_022c4994;
s32 OS_GetLockID(void);
void func_020588dc(void);
BOOL func_02058df0(void) { s32 id = OS_GetLockID(); if (id == -3) return FALSE; data_022c4994 = id; func_020588dc(); return TRUE; }
