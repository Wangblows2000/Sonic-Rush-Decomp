#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[0x24]; u8 unk28[1]; } UnkStruct_02062714;
BOOL TalkDemo_SetupBg2RockMotion(void *a);
void TalkDemoScene_State_RockPageWaitMotion(void);
void TalkDemoScene_State_RockPageMotion(UnkStruct_02062714 *s) { TalkDemo_SetupBg2RockMotion(s->unk28); s->fn = TalkDemoScene_State_RockPageWaitMotion; }

#pragma thumb reset
