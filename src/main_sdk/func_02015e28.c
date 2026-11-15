#include <nitro/types.h>

extern u8 *data_02080f1c;
void func_0201a8ac(void *p, void *q);
void func_020179dc(u32 a, u32 b, void *c);
void func_02015e28(u8 v, u32 b) { u8 t[8]; t[0] = v; func_0201a8ac(t, data_02080f1c); func_020179dc(6, b, data_02080f1c); }
