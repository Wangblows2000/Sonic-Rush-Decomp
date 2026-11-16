#include <nitro/types.h>

typedef struct { u8 unk0[0x474]; void (*fn)(void); u8 unk478[0x10]; u32 flags; } UnkStruct_0205aacc;
void StageTitleCard_State_BeginRestoreColors(void);
void StageTitleCard_State_WaitStartStage(UnkStruct_0205aacc *s) { if (s->flags & 0x40000) s->fn = StageTitleCard_State_BeginRestoreColors; }
