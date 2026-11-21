#include <nitro/types.h>

extern void *data_02078648[];
extern u8 data_02078b04;
void *WH_GetWMStateCodeName(u32 i) { if (i < 23) return data_02078648[i]; return &data_02078b04; }
