#include <nitro/types.h>

void VramPltt_Free(u32 a);
s32 VramPltt_GfdFree(u32 v) { VramPltt_Free((v & 0xffff) << 3); return 0; }
