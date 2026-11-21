#include <nitro/types.h>

u8 *NetPacket_FindNext(void);
u8 *NetPacket_FindNextData(void) { u8 *p = NetPacket_FindNext(); if (p == NULL) return NULL; return p + 4; }
