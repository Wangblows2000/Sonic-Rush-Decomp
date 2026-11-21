#include <nitro/types.h>

void OSi_AllocateCartridgeBus(void) { *(vu16 *)0x4000204 &= ~0x80; }
