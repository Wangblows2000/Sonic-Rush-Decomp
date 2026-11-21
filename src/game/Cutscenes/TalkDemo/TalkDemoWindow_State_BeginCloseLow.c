#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u32 unk4; u16 unk8; } UnkStruct_0206004c;
void TalkDemoWindow_State_CloseLow(void);
void TalkDemoWindow_State_BeginCloseLow(UnkStruct_0206004c *s) { s->unk8 = 0; s->fn = TalkDemoWindow_State_CloseLow; }

#pragma thumb reset
