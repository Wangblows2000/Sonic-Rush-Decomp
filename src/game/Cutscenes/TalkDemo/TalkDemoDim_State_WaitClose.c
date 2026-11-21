#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u16 unk4; } UnkStruct_0205f8b0;
u32 TalkDemo_GetFlags(s32 i);
void TalkDemo_ClearFlags(s32 i, u32 v);
void TalkDemo_SetFlags(s32 i, u32 v);
void TalkDemoDim_State_FadeOut(void);
void TalkDemoDim_State_WaitClose(UnkStruct_0205f8b0 *s) { if (TalkDemo_GetFlags(2) & 0x10) { TalkDemo_ClearFlags(2, 0x10); TalkDemo_SetFlags(2, 0x20); s->unk4 = 0; s->fn = TalkDemoDim_State_FadeOut; } }

#pragma thumb reset
