#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x340]; void (*fn)(void); u32 flags; } UnkStruct_02047e20;
typedef struct { u8 unk0[0x10]; s32 unk10; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
void ClearDemo_State_WaitParts(void);
void ClearDemoTA_SkipToRank(UnkStruct_02047e20 *s);
void ClearDemoScore_SkipToRank(UnkStruct_02047e20 *s);
void ClearDemo_SkipToRank(UnkStruct_02047e20 *s) { s->flags |= 0xa00000; s->fn = ClearDemo_State_WaitParts; if (gGameState.unk10 == 2) { ClearDemoTA_SkipToRank(s); return; } ClearDemoScore_SkipToRank(s); }

#pragma thumb reset
