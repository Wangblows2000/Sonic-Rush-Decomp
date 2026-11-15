#include <nitro/types.h>

extern u8 data_0207f02c[];
void func_0200d18c(u32 m) { while (*(vu16 *)(data_0207f02c + 0x36) & m) {} }
