#include <nitro/types.h>

#pragma thumb on

extern u16 gPad;
u32 TalkDemo_GetFlags(s32 i);
void TalkDemo_SetFlags(s32 i, u32 v);
BOOL Demo_IsSkipAreaTouched(void);
void TalkDemo_CheckSkip(void) { if (TalkDemo_GetFlags(0) & 0x100) { if ((gPad & 8) || Demo_IsSkipAreaTouched()) TalkDemo_SetFlags(0, 0x200); } }

#pragma thumb reset
