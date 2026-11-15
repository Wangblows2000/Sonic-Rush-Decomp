#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x340]; void (*fn)(void); u32 flags; u8 unk348[0x12e4]; u16 unk162C; } UnkStruct_02045cd0;
void func_02045c8c(void);
void func_02045cd0(UnkStruct_02045cd0 *s) { if (s->unk162C++ > 16) { s->flags |= 8; s->fn = func_02045c8c; s->unk162C = 0; } }

#pragma thumb reset
