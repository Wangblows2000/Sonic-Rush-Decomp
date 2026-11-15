#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x340]; void (*fn)(void); u32 flags; } UnkStruct_02047e20;
typedef struct { u8 unk0[0x10]; s32 unk10; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
void func_02045c28(void);
void func_020453f8(UnkStruct_02047e20 *s);
void func_0204609c(UnkStruct_02047e20 *s);
void func_02047e20(UnkStruct_02047e20 *s) { s->flags |= 0xa00000; s->fn = func_02045c28; if (data_022c4560.unk10 == 2) { func_020453f8(s); return; } func_0204609c(s); }

#pragma thumb reset
