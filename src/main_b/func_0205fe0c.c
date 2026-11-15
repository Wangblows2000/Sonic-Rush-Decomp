#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u32 unk4; u16 unk8; } UnkStruct_0205fe0c;
void func_0205fd9c(void);
void func_0205fe0c(UnkStruct_0205fe0c *s) { s->unk8 = 0; s->fn = func_0205fd9c; }

#pragma thumb reset
