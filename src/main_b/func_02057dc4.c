#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 flags; u8 unk8[0x18]; u16 unk20; } UnkStruct_02057dc4;
void func_02057dc4(UnkStruct_02057dc4 *s) { s->flags |= 2; s->unk20 = 0; }

#pragma thumb reset
