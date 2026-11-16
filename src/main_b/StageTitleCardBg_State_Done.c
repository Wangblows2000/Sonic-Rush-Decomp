#include <nitro/types.h>

typedef struct { u8 unk0[0x488]; u32 flags; } UnkStruct_0205c614;
void StageTitleCardBg_State_Done(UnkStruct_0205c614 *s) { s->flags |= 2; }
