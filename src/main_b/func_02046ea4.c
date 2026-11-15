#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x344]; u32 flags; } UnkStruct_02046ea4;
void func_02046ea4(UnkStruct_02046ea4 *s) { s->flags |= 0x800; }

#pragma thumb reset
