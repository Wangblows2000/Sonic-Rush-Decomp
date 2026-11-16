#include <nitro/types.h>

extern u8 data_0207f0e4[];
void RtcGetResultCallback(u32 v) { *(u32 *)(data_0207f0e4 + 0x20) = v; }
