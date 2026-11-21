#include <nitro/types.h>

typedef struct { u8 unk0[0x2c]; u32 unk2C; } UnkStruct_020698bc;
void SpritePair_SetOAMPriority(UnkStruct_020698bc *a, UnkStruct_020698bc *b, u32 v) { a->unk2C &= ~0x60000; b->unk2C &= ~0x60000; a->unk2C |= v << 17; b->unk2C |= v << 17; }
