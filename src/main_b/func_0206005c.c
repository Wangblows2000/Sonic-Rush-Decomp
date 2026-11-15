#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u32 unk4; u16 unk8; } UnkStruct_0206005c;
u32 func_02060e70(s32 i);
void func_02060e38(s32 i, u32 v);
void func_02060e54(s32 i, u32 v);
void func_0206004c(void);
void func_0206005c(UnkStruct_0206005c *s) { if (func_02060e70(1) & 0x10) { func_02060e38(1, 0x10); func_02060e54(1, 0x20); s->unk8 = 0; s->fn = func_0206004c; } }

#pragma thumb reset
