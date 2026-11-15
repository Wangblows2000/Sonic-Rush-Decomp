#include <nitro/types.h>

void func_020405e0(s32 a);
void func_0203ebc8(void);
s32 func_02011b10(void (*cb)(void));
void func_020405bc(s32 v);
BOOL func_0203ec20(void) { s32 r; func_020405e0(3); r = func_02011b10(func_0203ebc8); if (r == 2) return TRUE; func_020405bc(r); return FALSE; }
