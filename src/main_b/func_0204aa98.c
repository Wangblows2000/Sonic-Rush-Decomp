#include <nitro/types.h>

typedef struct { u8 unk0[0x5c]; u32 unk5C; } UnkStruct_0204aa98;
extern u32 data_022c4484;
void func_0204aa98(UnkStruct_0204aa98 *s) { data_022c4484 = s->unk5C; }
