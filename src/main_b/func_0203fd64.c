#include <nitro/types.h>

extern u8 data_022c1a40;
void func_020405e0(s32 a);
void func_0203fd30(void);
s32 func_02012da0(void (*cb)(void), void *p);
void func_020405bc(s32 v);
BOOL func_0203fd64(void) { s32 r; func_020405e0(6); r = func_02012da0(func_0203fd30, &data_022c1a40); if (r == 2) return TRUE; func_020405bc(r); return FALSE; }
