#include <nitro/types.h>

extern u8 data_022c1a00;
void func_020405e0(s32 a);
void func_02040378(void);
s32 func_020119c0(void (*cb)(void), void *p);
void func_020405bc(s32 v);
BOOL func_02040408(void) { s32 r; func_020405e0(3); r = func_020119c0(func_02040378, &data_022c1a00); if (r == 2) return TRUE; func_020405bc(r); func_020405e0(9); return FALSE; }
