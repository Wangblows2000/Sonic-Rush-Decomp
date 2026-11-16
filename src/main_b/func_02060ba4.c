#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u32 flags; } UnkStruct_02060ba4;
u32 TalkDemo_GetFlags(s32 i);
void TalkDemo_SetFlags(s32 i, u32 v);
void func_02060ba4(UnkStruct_02060ba4 *s) { if (TalkDemo_GetFlags(1) & 0x40) { TalkDemo_SetFlags(3, 0x40000000); s->fn = NULL; s->flags |= 0x80000000; } }

#pragma thumb reset
