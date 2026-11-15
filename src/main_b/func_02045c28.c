#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x340]; void (*fn)(void); u32 flags; } UnkStruct_02045c28;
void func_02045bf8(void);
void func_02045c28(UnkStruct_02045c28 *s) { if ((s->flags & 0x88a90) == 0x88a90) s->fn = func_02045bf8; }

#pragma thumb reset
