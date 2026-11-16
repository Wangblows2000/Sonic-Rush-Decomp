#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[0x62]; u16 unk66; } UnkStruct_02046f78;
void ClearDemoActText_State_Delay(void);
void ClearDemoActText_State_BeginDelay(void *a, UnkStruct_02046f78 *s) { s->unk66 = 0; s->fn = ClearDemoActText_State_Delay; }

#pragma thumb reset
