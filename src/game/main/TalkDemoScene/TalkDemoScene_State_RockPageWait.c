#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u32 unk4; u32 unk8; s32 unkC; } UnkStruct_02062664;
void TalkDemoScene_State_RockPageEnd(void);
void TalkDemoScene_State_RockPageWait(UnkStruct_02062664 *s) { s->unkC--; if (s->unkC <= 0) s->fn = TalkDemoScene_State_RockPageEnd; }

#pragma thumb reset
