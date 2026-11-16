#include <nitro/types.h>

extern u8 data_0207f02c[];
u16 TP_GetLatestIndexInAuto(void) { return *(u16 *)(data_0207f02c + 0xc); }
