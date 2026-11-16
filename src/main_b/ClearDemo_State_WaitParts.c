#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x340]; void (*fn)(void); u32 flags; } UnkStruct_02045c28;
void ClearDemo_State_Hold(void);
void ClearDemo_State_WaitParts(UnkStruct_02045c28 *s) { if ((s->flags & 0x88a90) == 0x88a90) s->fn = ClearDemo_State_Hold; }

#pragma thumb reset
