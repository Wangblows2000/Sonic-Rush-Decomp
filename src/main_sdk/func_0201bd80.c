#include <nitro/types.h>

typedef struct { u8 pad[0x3c]; u8 h; } UnkStruct_0201bd80;
void func_02008e04(u32 a, u32 b);
void func_0201bd80(UnkStruct_0201bd80 **pp, u32 b) { if (*pp == 0) return; func_02008e04((*pp)->h, b); }
