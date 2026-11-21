#include <nitro/types.h>

typedef struct { u8 unk0[0x474]; void (*fn)(void); u8 unk478[0x454]; u16 unk8CC; u16 unk8CE; } UnkStruct_0205b1c4;
void StageTitleCard_State_HoldText(void);
void StageTitleCard_State_BeginHoldText(UnkStruct_0205b1c4 *s) { s->unk8CC = 0; s->fn = StageTitleCard_State_HoldText; }
