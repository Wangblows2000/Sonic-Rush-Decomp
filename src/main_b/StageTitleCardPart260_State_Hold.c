#include <nitro/types.h>

typedef struct { u8 unk0[0x488]; u32 flags; } UnkStruct_0205bd4c;
typedef struct { void (*fn)(void); u8 unk4[0xa0]; s32 unkA4; } UnkObj_0205bd4c;
void StageTitleCardPart260_State_BeginExit(void);
void StageTitleCardPart260_State_Hold(UnkStruct_0205bd4c *s, UnkObj_0205bd4c *o) { o->unkA4 += -0x333; if (s->flags & 0x10000000) o->fn = StageTitleCardPart260_State_BeginExit; }
