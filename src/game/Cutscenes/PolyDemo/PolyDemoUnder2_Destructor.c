#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u32 unk10; } UnkStruct_02057c9c;
void PolyDemoUnder2_ReleaseSprites(u32 a);
void PolyDemoUnder2_Destructor(UnkStruct_02057c9c *s) { PolyDemoUnder2_ReleaseSprites(s->unk10); }

#pragma thumb reset
