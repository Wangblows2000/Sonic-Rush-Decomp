#include <nitro/types.h>

extern u8 data_02078b14;
extern void *data_022c4480;
void *func_02033fbc(void *a, s32 b);
void func_0204525c(void) { data_022c4480 = func_02033fbc(&data_02078b14, 0); }
