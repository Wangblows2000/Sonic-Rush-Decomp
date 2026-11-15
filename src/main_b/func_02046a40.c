#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x344]; u32 flags; } UnkStruct_02046a40;
void func_02046a40(UnkStruct_02046a40 *s) { s->flags |= 0x80; }

#pragma thumb reset
