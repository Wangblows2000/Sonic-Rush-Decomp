#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x344]; u32 flags; } UnkStruct_0204714c;
void ClearDemoName_State_Done(UnkStruct_0204714c *s) { s->flags |= 0x200; }

#pragma thumb reset
