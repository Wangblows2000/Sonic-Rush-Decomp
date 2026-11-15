#include <nitro/types.h>

typedef struct { u8 unk0[0x5c]; u32 unk5C; } UnkStruct_0204aecc;
extern u32 data_ov000_0236aa8c;
void func_0204aecc(UnkStruct_0204aecc *s) { data_ov000_0236aa8c = s->unk5C; s->unk5C = 0; }
