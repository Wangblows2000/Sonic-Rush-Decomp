#include <nitro/types.h>

typedef struct { u8 unk0[0x474]; void (*fn)(void); u8 unk478[0x454]; u16 unk8CC; u16 unk8CE; } UnkStruct_0205b35c;
void StageTitleCard_State_FadeIn(void);
void StageTitleCard_State_BeginFadeIn(UnkStruct_0205b35c *s) { s->unk8CE = 0; s->fn = StageTitleCard_State_FadeIn; }
