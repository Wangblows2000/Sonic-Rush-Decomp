#include <nitro/types.h>

extern u8 data_02080f20[];
void MBi_SetMaxScanTime(u16 v) { *(u16 *)(data_02080f20 + 6) = v; }
