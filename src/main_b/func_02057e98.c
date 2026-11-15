#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 flags; } UnkStruct_02057e98;
void func_02057e98(UnkStruct_02057e98 *s) { s->flags |= 0x20; }

#pragma thumb reset
