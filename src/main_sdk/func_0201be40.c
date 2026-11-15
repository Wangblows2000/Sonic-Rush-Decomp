#include <nitro/types.h>

typedef struct { u8 pad[0x1c]; u8 x1c[0x10]; u8 x2c; } UnkStruct_0201be40;
void func_020205a0(void *p, s32 v, u32 c);
void func_0201be40(UnkStruct_0201be40 **pp, s32 v, u32 c) { UnkStruct_0201be40 *p = *pp; if (p == 0) return; if (p->x2c == 2) return; func_020205a0(p->x1c, v << 8, c); }
