#include <nitro/types.h>

extern u8 data_022c2440;
extern u8 data_022c1ba0;
void func_02011f88(void *a, void *b, void *c);
void WH_GetSharedDataAdr(void *c) { func_02011f88(&data_022c2440, &data_022c1ba0, c); }
