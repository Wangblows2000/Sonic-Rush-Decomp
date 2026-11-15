#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 flags; } UnkStruct_02057da4;
void func_02057da4(UnkStruct_02057da4 *s) { s->flags |= 0x8000; }

#pragma thumb reset
