#include <nitro/types.h>

typedef struct { u8 pad[0x2c]; u8 active; u8 pad2[0x17]; } UnkStruct_0201bfbc;
extern UnkStruct_0201bfbc data_02083464[];
void func_0201b98c(UnkStruct_0201bfbc *p, u32 a);
void func_0201bfbc(u32 a) { s32 i; UnkStruct_0201bfbc *p = data_02083464; for (i = 0; i < 16; i++, p++) { if (p->active) func_0201b98c(p, a); } }
