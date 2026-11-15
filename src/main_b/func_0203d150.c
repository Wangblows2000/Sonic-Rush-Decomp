#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; u32 unk8; u32 unkC; } UnkStruct_022b8be0;
extern UnkStruct_022b8be0 data_022b8be0;
void func_0203d644(u32 v);
void func_0203d190(void);
void func_0203d150(void) { if (data_022b8be0.unkC == 0) return; data_022b8be0.unk4 = 1; func_0203d644(1); func_0203d190(); }
