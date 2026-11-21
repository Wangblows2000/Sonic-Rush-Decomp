#include <nitro/types.h>

typedef struct { u8 unk0[0x474]; void (*fn)(void); u8 unk478[0x454]; u16 unk8CC; } UnkStruct_0205b290;
void StageTitleCard_State_ShowParts(void);
void StageTitleCard_State_ShowDelay(UnkStruct_0205b290 *s) { if (s->unk8CC++ != 0) s->fn = StageTitleCard_State_ShowParts; }
