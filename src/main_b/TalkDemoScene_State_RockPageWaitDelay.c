#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u32 unk4; u32 unk8; s32 unkC; } UnkStruct_020626d0;
void TalkDemoScene_State_RockPageSpawn(void);
void TalkDemoScene_State_RockPageWaitDelay(UnkStruct_020626d0 *s) { s->unkC--; if (s->unkC <= 0) s->fn = TalkDemoScene_State_RockPageSpawn; }

#pragma thumb reset
