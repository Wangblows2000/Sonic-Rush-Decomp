#include <nitro/types.h>

extern u32 data_0207efb8;
void func_0200bf48(u32 a);
void func_0200be98(u32 a) { if (data_0207efb8 != 0) return; data_0207efb8 = 1; func_0200bf48(a); }
