#include <nitro/types.h>

typedef struct { u8 unk0[0x488]; u32 flags; } UnkStruct_0205c038;
void StageTitleCardPart180_State_Done(UnkStruct_0205c038 *s) { s->flags |= 0x100; }
