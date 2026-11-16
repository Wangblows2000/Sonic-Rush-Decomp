#include <nitro/types.h>

typedef struct UnkStruct_0201541c { u8 pad[0x28]; struct UnkStruct_0201541c *next; } UnkStruct_0201541c;
UnkStruct_0201541c *func_0201558c(void);
s32 func_0201541c(void) { s32 n = 0; UnkStruct_0201541c *p; for (p = func_0201558c(); p != 0; p = p->next) n++; return n; }
