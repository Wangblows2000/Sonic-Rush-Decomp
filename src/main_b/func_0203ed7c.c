#include <nitro/types.h>

extern s32 data_022c1980;
void func_020405e0(s32 a);
void func_0203fc88(void);
s32 func_02012d20(void (*cb)(void));
void func_020405bc(s32 v);
BOOL func_0203ed7c(void) { s32 r; if (data_022c1980 != 6) return FALSE; func_020405e0(3); r = func_02012d20(func_0203fc88); if (r == 2) return TRUE; func_020405bc(r); return FALSE; }
