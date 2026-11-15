#include <nitro/types.h>

typedef struct { u8 pad[0x34]; u16 a; u16 pad2; u16 b; } UnkStruct_0201bd64;
void func_0201bd64(UnkStruct_0201bd64 **pp, u16 v) { if (*pp != 0) { (*pp)->a = 1; (*pp)->b = v; } }
