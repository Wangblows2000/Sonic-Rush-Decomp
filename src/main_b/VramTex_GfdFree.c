#include <nitro/types.h>

void VramTex_Free(u32 a);
s32 VramTex_GfdFree(u32 v) { VramTex_Free((v & 0xffff) << 3); return 0; }
