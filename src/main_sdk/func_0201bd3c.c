#include <nitro/types.h>

typedef struct { u8 pad[0x34]; u16 a; u16 pad2; u16 b; u16 c; } UnkStruct_0201bd3c;
void func_0201bd3c(UnkStruct_0201bd3c **pp, u16 b, u16 c) { if (*pp == 0) return; (*pp)->a = 2; (*pp)->b = b; (*pp)->c = c; }
