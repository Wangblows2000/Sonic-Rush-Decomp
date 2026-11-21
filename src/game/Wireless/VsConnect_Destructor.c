#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u32 unk10; } UnkStruct_02063400;
void VsConnect_Release(u32 a);
void VsConnect_Destructor(UnkStruct_02063400 *s) { VsConnect_Release(s->unk10); }

#pragma thumb reset
