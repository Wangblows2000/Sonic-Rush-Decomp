#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u32 flags; } UnkStruct_02060ba4;
u32 func_02060e70(s32 i);
void func_02060e54(s32 i, u32 v);
void func_02060ba4(UnkStruct_02060ba4 *s) { if (func_02060e70(1) & 0x40) { func_02060e54(3, 0x40000000); s->fn = NULL; s->flags |= 0x80000000; } }

#pragma thumb reset
