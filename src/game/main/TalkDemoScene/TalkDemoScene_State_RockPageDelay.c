#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u32 unk4; u32 unk8; u32 unkC; } UnkStruct_020626e8;
void TalkDemoScene_State_RockPageWaitDelay(void);
void TalkDemoScene_State_RockPageDelay(UnkStruct_020626e8 *s) { s->unkC = 10; s->fn = TalkDemoScene_State_RockPageWaitDelay; }

#pragma thumb reset
