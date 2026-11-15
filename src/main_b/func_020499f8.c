#include <nitro/types.h>

extern void *data_022c4488;
extern u8 data_0207a070;
void func_02021310(void *a, void *b, void *c);
void func_020212e0(void *a);
void func_020499f8(void) { u8 buf[0x5c]; func_02021310(buf, &data_0207a070, data_022c4488); func_020212e0(buf); }
