#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x340]; void (*fn)(void); u32 flags; } UnkStruct_02047e20;
typedef struct { u8 unk0[0x10]; s32 unk10; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
void ClearDemo_Unk_02045c28(void);
void ClearDemo_Unk_020453f8(UnkStruct_02047e20 *s);
void ClearDemo_Unk_0204609c(UnkStruct_02047e20 *s);
void ClearDemo_Unk_02047e20(UnkStruct_02047e20 *s) { s->flags |= 0xa00000; s->fn = ClearDemo_Unk_02045c28; if (gGameState.unk10 == 2) { ClearDemo_Unk_020453f8(s); return; } ClearDemo_Unk_0204609c(s); }

#pragma thumb reset
