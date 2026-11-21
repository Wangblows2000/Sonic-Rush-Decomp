#include <nitro/types.h>

typedef struct { u8 unk0[0x474]; void (*fn)(void); u8 unk478[0x10]; u32 flags; } UnkStruct_0205b1e0;
void StageTitleCard_State_BeginHoldText(void);
void StageTitleCard_State_WaitText(UnkStruct_0205b1e0 *s) { if (s->flags & 0x4000) s->fn = StageTitleCard_State_BeginHoldText; }
