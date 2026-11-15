#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; u32 unk8; u32 unkC; u8 unk10[0x8844]; u32 unk8854; } UnkStruct_022b8be0;
extern UnkStruct_022b8be0 data_022b8be0;
void func_0203d644(void);
void func_0203d930(void) { UnkStruct_022b8be0 *p = &data_022b8be0; if (p->unkC == 0) return; if (p->unk0 == 0) return; p->unk8854 = 0; func_0203d644(); }
