#include <nitro/types.h>

typedef struct { u8 unk0[0x488]; u32 flags; } UnkStruct_0205b8ec;
void StageTitleCardPart324_State_Done(UnkStruct_0205b8ec *s) { s->flags |= 0x1000; }
