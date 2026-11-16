#include <nitro/types.h>

typedef struct { u8 unk0[0x488]; u32 flags; } UnkStruct_0205b74c;
void StageTitleCardText3E8_State_Done(UnkStruct_0205b74c *s) { s->flags |= 0x4000; }
