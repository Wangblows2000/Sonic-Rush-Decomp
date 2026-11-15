// cc: 1.2/sp2p3
#include <nitro/types.h>

typedef struct { u8 pad[0x3c]; u8 h; } UnkStruct_0201bde0;
void func_02008dac(u32 a, u32 b, u32 c);
void func_0201bde0(UnkStruct_0201bde0 **pp, u32 b, u32 c) { if (*pp == 0) return; func_02008dac((*pp)->h, b, c); }
