#include <nitro/types.h>

extern u8 data_022c2440;
extern u8 data_022c1ba0;
void WM_GetSharedDataAddress(void *a, void *b, void *c);
void WH_GetSharedDataAdr(void *c) { WM_GetSharedDataAddress(&data_022c2440, &data_022c1ba0, c); }
