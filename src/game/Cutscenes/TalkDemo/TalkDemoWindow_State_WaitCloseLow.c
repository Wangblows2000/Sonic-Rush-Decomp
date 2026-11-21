#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u32 unk4; u16 unk8; } UnkStruct_0206005c;
u32 TalkDemo_GetFlags(s32 i);
void TalkDemo_ClearFlags(s32 i, u32 v);
void TalkDemo_SetFlags(s32 i, u32 v);
void TalkDemoWindow_State_BeginCloseLow(void);
void TalkDemoWindow_State_WaitCloseLow(UnkStruct_0206005c *s) { if (TalkDemo_GetFlags(1) & 0x10) { TalkDemo_ClearFlags(1, 0x10); TalkDemo_SetFlags(1, 0x20); s->unk8 = 0; s->fn = TalkDemoWindow_State_BeginCloseLow; } }

#pragma thumb reset
