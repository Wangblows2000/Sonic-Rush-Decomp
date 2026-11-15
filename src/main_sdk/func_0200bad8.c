#include <nitro/types.h>

typedef struct { u8 pad[8]; u32 x8; u32 xc; u32 x10; } UnkStruct_0200bad8;
BOOL func_0200b384(UnkStruct_0200bad8 *p, u32 a);
BOOL func_0200bad8(UnkStruct_0200bad8 *p) { if (!func_0200b384(p, 8)) return 0; p->x8 = 0; p->x10 = 0xe; p->xc &= ~0x30; return 1; }
