#include <nitro/types.h>

typedef struct { u8 unk0[0x488]; u32 flags; } UnkStruct_0205c4dc;
void StageTitleCardStrip_State_Done(UnkStruct_0205c4dc *s) { s->flags |= 8; }
