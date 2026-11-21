#include <nitro/types.h>

typedef struct { u8 unk0[0x474]; void (*fn)(void); u8 unk478[0x454]; u16 unk8CC; } UnkStruct_0205b228;
void StageTitleCard_State_ShowText(void);
void StageTitleCard_State_Hold(UnkStruct_0205b228 *s) { if (s->unk8CC++ == 30) s->fn = StageTitleCard_State_ShowText; }
