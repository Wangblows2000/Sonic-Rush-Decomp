#include <nitro/types.h>

typedef struct { u8 unk0[0x474]; void (*fn)(void); u8 unk478[0x10]; u32 flags; u32 flags2; } UnkStruct_0205b268;
void StageTitleCard_State_BeginHold(void);
void StageTitleCard_State_ShowParts(UnkStruct_0205b268 *s) { s->flags |= 0xa80; s->flags2 |= 0x70; s->fn = StageTitleCard_State_BeginHold; }
