#include <nitro/types.h>

extern u16 sParentPacketSize;
extern u16 sChildPacketSize;
void WH_SetMaxParentChildSize(u16 a, u16 b) { sParentPacketSize = a; sChildPacketSize = b; }
