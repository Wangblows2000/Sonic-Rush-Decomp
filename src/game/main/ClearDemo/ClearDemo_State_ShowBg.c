#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x340]; void (*fn)(void); u32 flags; u8 unk348[0x12e4]; u16 unk162C; } UnkStruct_02045cd0;
void ClearDemo_State_ShowName(void);
void ClearDemo_State_ShowBg(UnkStruct_02045cd0 *s) { if (s->unk162C++ > 16) { s->flags |= 8; s->fn = ClearDemo_State_ShowName; s->unk162C = 0; } }

#pragma thumb reset
