#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x340]; void (*fn)(void); u32 flags; u8 unk348[0x12e4]; u16 unk162C; } UnkStruct_02045c4c;
void func_02045c28(void);
void func_02045c4c(UnkStruct_02045c4c *s) { if (s->unk162C++ > 8) { s->flags |= 0x20; s->fn = func_02045c28; s->unk162C = 0; } }

#pragma thumb reset
