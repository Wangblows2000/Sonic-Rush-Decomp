#include <nitro/types.h>

typedef struct { u8 pad[0x2e]; u8 x2e; u8 pad2[0xd]; u8 x3c; } UnkStruct_0201b944;
void func_02008e1c(u32 a, u32 b);
void func_0201b944(UnkStruct_0201b944 *p, u32 v) { if (p == 0) return; if (v == p->x2e) return; func_02008e1c(p->x3c, v); p->x2e = v; }
