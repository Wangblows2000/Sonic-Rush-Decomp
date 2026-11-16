#include <nitro/types.h>

typedef struct { u32 unk0; u16 unk4; u32 unk8; } UnkStruct_022c1a00;
extern UnkStruct_022c1a00 sParentParam;
void WH_SetGgid(u32 v) { sParentParam.unk8 = v; }
