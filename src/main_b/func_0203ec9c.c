#include <nitro/types.h>

void func_020405e0(s32 a);
void func_0203ec64(void);
s32 func_02011598(void (*cb)(void), s32 a);
void func_020405bc(s32 v);
void func_0203dda4(void);
BOOL func_0203ec9c(void) { s32 r; func_020405e0(3); r = func_02011598(func_0203ec64, 0); if (r == 2) return TRUE; func_020405bc(r); func_0203dda4(); return FALSE; }
