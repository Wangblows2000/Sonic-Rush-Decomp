#include <nitro/types.h>

typedef struct { u8 unk0[0x474]; void (*fn)(void); u8 unk478[0x10]; u32 flags; } UnkStruct_0205b148;
void StageTitleCard_State_BeginExit(void);
void StageTitleCard_State_WaitText2(UnkStruct_0205b148 *s) { if (s->flags & 0x10000) s->fn = StageTitleCard_State_BeginExit; }
