#include <nitro/types.h>

typedef struct { u8 unk0[0x46]; s16 unk46; } UnkStruct_0206990c;
void SpritePair_SetOAMOrder(UnkStruct_0206990c *a, UnkStruct_0206990c *b, s16 v) { a->unk46 = v; b->unk46 = v; }
