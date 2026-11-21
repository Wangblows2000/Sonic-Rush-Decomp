#include <nitro/types.h>

typedef struct { u8 unk0[0x488]; u32 flags; } UnkStruct_0205b564;
void StageTitleCardPart420_State_Done(UnkStruct_0205b564 *s) { s->flags |= 0x40000; }
