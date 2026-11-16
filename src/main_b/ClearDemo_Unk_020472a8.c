#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x344]; u32 flags; } UnkStruct_020472a8;
void ClearDemo_Unk_020472a8(UnkStruct_020472a8 *s) { s->flags |= 0x10; }

#pragma thumb reset
