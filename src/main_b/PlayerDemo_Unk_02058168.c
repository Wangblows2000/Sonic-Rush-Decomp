#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 flags; } UnkStruct_02058168;
void PlayerDemo_Unk_02058168(UnkStruct_02058168 *s) { s->unk0 = 0; s->flags |= 0x80000000; }

#pragma thumb reset
