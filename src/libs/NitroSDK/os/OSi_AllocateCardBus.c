#include <nitro/types.h>

void OSi_AllocateCardBus(void) { *(vu16 *)0x4000204 &= ~0x800; }
