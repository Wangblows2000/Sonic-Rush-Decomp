#include <nitro/types.h>

typedef struct { u8 unk0[0x488]; u32 flags; } UnkStruct_0205bc64;
void StageTitleCardPart260_State_Done(UnkStruct_0205bc64 *s) { s->flags |= 0x400; }
