#include <nitro/types.h>

extern u8 data_0207f02c[];
void TP_WaitBusy(u32 m) { while (*(vu16 *)(data_0207f02c + 0x36) & m) {} }
