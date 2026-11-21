#include <nitro/types.h>

typedef struct { u8 pad[8]; u32 ramSize; u8 pad2[0x10]; u32 compressed : 24; u32 flag : 8; } UnkStruct_0200c894;
u32 FSi_GetOverlayBinarySize(UnkStruct_0200c894 *p) { return (p->flag & 1) ? p->compressed : p->ramSize; }
