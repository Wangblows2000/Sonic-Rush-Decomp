#include <nitro/types.h>

extern u8 data_022c2c60;
void func_020405e0(s32 a);
void func_0203e388(void);
s32 func_02011b50(void *p, void (*cb)(void), s32 n);
void func_020405bc(s32 v);
BOOL func_0203e3f4(void) { s32 r; func_020405e0(3); r = func_02011b50(&data_022c2c60, func_0203e388, 2); if (r == 2) return TRUE; func_020405bc(r); func_020405e0(10); return FALSE; }
