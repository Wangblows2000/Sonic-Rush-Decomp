#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u32 unk4; u16 unk8; } UnkStruct_0205fe0c;
void TalkDemoWindow_State_CloseCenter(void);
void TalkDemoWindow_State_BeginCloseCenter(UnkStruct_0205fe0c *s) { s->unk8 = 0; s->fn = TalkDemoWindow_State_CloseCenter; }

#pragma thumb reset
