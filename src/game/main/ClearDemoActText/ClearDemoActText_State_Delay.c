#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[0x62]; u16 unk66; } UnkStruct_02046f54;
void ClearDemoActText_State_Spread(void);
void ClearDemoActText_State_Delay(void *a, UnkStruct_02046f54 *s) { if (s->unk66++ > 15) s->fn = ClearDemoActText_State_Spread; }

#pragma thumb reset
