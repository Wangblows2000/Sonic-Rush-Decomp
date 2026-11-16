#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x340]; void (*fn)(void); u32 flags; u8 unk348[0x12e4]; u16 unk162C; } UnkStruct_02045c8c;
void ClearDemo_Unk_02045c4c(void);
void ClearDemo_Unk_02045c8c(UnkStruct_02045c8c *s) { if (s->unk162C++ > 8) { s->flags |= 0x100; s->fn = ClearDemo_Unk_02045c4c; s->unk162C = 0; } }

#pragma thumb reset
