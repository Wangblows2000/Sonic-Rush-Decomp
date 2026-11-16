#include <nitro/types.h>

typedef struct { u8 unk0[0x474]; void (*fn)(void); u8 unk478[0x454]; u16 unk8CC; } UnkStruct_0205b1a0;
void StageTitleCard_State_ShowText2(void);
void StageTitleCard_State_HoldText(UnkStruct_0205b1a0 *s) { if (s->unk8CC++ == 45) s->fn = StageTitleCard_State_ShowText2; }
