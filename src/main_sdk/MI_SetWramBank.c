#include <nitro/types.h>

void MI_SetWramBank(u8 v) { *(vu8 *)0x4000247 = v; }
