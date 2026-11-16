#include <nitro/types.h>

typedef struct { u8 unk0[0x488]; u32 flags; } UnkStruct_0205b5ac;
void StageTitleCardText404_State_Done(UnkStruct_0205b5ac *s) { s->flags |= 0x10000; }
