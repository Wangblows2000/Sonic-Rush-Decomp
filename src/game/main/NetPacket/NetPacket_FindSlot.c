#include <nitro/types.h>

extern u8 data_0207cfb4[16];
u16 NetPacket_FindSlot(u8 v) { u16 i; for (i = 0; i < 16; i++) { if (data_0207cfb4[i] == v) return i; } return 0xffff; }
