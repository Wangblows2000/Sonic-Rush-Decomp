#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x344]; u32 flags; } UnkStruct_02045b7c;
void ClearDemo_State_Finish(UnkStruct_02045b7c *s) { s->flags |= 0x80000000; }

#pragma thumb reset
