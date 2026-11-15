#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u32 unk4; u16 unk8; } UnkStruct_0206004c;
void func_0205fff8(void);
void func_0206004c(UnkStruct_0206004c *s) { s->unk8 = 0; s->fn = func_0205fff8; }

#pragma thumb reset
