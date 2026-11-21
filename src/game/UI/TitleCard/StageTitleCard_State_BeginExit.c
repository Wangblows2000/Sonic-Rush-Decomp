#include <nitro/types.h>

typedef struct { u8 unk0[0x474]; void (*fn)(void); u8 unk478[0x10]; u32 flags; u32 flags2; u8 unk490[0x43c]; u16 unk8CC; u16 unk8CE; } UnkStruct_0205b120;
void StageTitleCard_State_Exit(void);
void StageTitleCard_State_BeginExit(UnkStruct_0205b120 *s) { s->flags |= 0x10000000; s->unk8CE = 0; s->fn = StageTitleCard_State_Exit; }
