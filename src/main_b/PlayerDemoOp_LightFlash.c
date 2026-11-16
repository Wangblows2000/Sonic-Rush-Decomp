#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 flags; u8 unk8[0x20]; u16 unk28; } UnkStruct_02057db4;
void PlayerDemoOp_LightFlash(UnkStruct_02057db4 *s, u16 v) { s->flags |= 0x1000; s->unk28 = v; }

#pragma thumb reset
