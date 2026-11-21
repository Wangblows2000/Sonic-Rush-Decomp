#include <nitro/types.h>

typedef struct { u8 unk0[0x474]; void (*fn)(void); u8 unk478[0x454]; u16 unk8CC; } UnkStruct_0205b378;
void StageTitleCard_State_BeginFadeIn(void);
void StageTitleCard_State_StartDelay(UnkStruct_0205b378 *s) { if (s->unk8CC++ > 5) s->fn = StageTitleCard_State_BeginFadeIn; }
