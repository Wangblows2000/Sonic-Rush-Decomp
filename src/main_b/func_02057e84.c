#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 flags; } UnkStruct_02057e84;
void func_02057e84(UnkStruct_02057e84 *s) { s->flags |= 0x40; s->flags |= 0x10; }

#pragma thumb reset
