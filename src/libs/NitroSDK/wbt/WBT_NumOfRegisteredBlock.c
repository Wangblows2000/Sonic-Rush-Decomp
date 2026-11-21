#include <nitro/types.h>

typedef struct UnkStruct_0201541c { u8 pad[0x28]; struct UnkStruct_0201541c *next; } UnkStruct_0201541c;
UnkStruct_0201541c *WBT_GetBlockListHead(void);
s32 WBT_NumOfRegisteredBlock(void) { s32 n = 0; UnkStruct_0201541c *p; for (p = WBT_GetBlockListHead(); p != 0; p = p->next) n++; return n; }
