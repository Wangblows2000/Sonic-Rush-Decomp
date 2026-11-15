#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u16 unk4; } UnkStruct_0205f8b0;
u32 func_02060e70(s32 i);
void func_02060e38(s32 i, u32 v);
void func_02060e54(s32 i, u32 v);
void func_0205f868(void);
void func_0205f8b0(UnkStruct_0205f8b0 *s) { if (func_02060e70(2) & 0x10) { func_02060e38(2, 0x10); func_02060e54(2, 0x20); s->unk4 = 0; s->fn = func_0205f868; } }

#pragma thumb reset
